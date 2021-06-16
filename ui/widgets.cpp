
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMessageBox>
#include <ctime>
#define __STDC_FORMAT_MACROS
#include <cinttypes>

#include "dwr.h"
#include "mt64.h"
#include "widgets.h"

CheckBox::CheckBox(const QString &text, QWidget *parent) :
        QCheckBox(text, parent)
{
    this->conflicts = "";
    this->requires = "";
    this->flagoffset = -1;
}

CheckBox::CheckBox(const QString &text,
        const QString requires, const QString conflicts, QWidget *parent,
        int flagoffset) :
        QCheckBox(text, parent)
{
    this->requires = requires;
    this->conflicts = conflicts;
    this->flagoffset = flagoffset;
}

void CheckBox::writeFlag(char* flags)
{
  if (this->flagoffset > -1) {
    int flagindex = this->flagoffset / 8;
    int offset = this->flagoffset % 8;
    char mask = 0x3 << offset;

    char newvalue = 0;
    if (this->checkState() == Qt::Checked) {
        newvalue = 1;
    }
    if (this->checkState() == Qt::PartiallyChecked) {
        newvalue = 2;
    }
    flags[flagindex] = (flags[flagindex] & (~mask)) | (newvalue << offset);
  }
}

bool CheckBox::updateState(const char *flags)
{
  if (this->flagoffset > -1) {
    int flagindex = this->flagoffset/8;
    int iso = ((0x3 << (this->flagoffset % 8)) & flags[flagindex]) >> (this->flagoffset % 8);

    if (iso == 0) {
      this->setCheckState(Qt::Unchecked);
    } else if (iso == 2) {
      this->setCheckState(Qt::PartiallyChecked);
    } else if (iso == 1) {
      this->setCheckState(Qt::Checked);
    } 
  }
  return true;
}

bool CheckBox::updateConflicts(const char *flags)
{
    bool enabled = true;

    this->updateState(flags);

    // actually update with conflicts/requires

    return enabled;
}

// bool CheckBox::updateConflicts(const QString flags)
// {
//     bool enabled = true;
//     bool checked = flags.contains(this->flag);

//     for (int i=0; i < this->requires.length(); i++) {
//         if (!flags.contains(this->requires.at(i))) {
//             enabled = false;
//             checked = false;
//             break;
//         }
//     }
//     if (enabled) {
//         for (int i=0; i < this->conflicts.length(); i++) {
//             if (flags.contains(this->conflicts.at(i))) {
//                 enabled = false;
//                 checked = false;
//                 break;
//             }
//         }
//     }
//     //this->setEnabled(enabled);
//     //this->setChecked(checked);
//     return enabled;
// }

LevelComboBox::LevelComboBox(int flagoffset, QWidget *parent) : QComboBox(parent)
{
    this->addItem("Normal");
    this->addItem("Fast");
    this->addItem("Very Fast");
    this->addItem("Random (Fast/Very Fast)");
    this->addItem("Random (All)");
    this->flagoffset = flagoffset;
}

void LevelComboBox::writeFlag(char *flags)
{
  if (this->flagoffset > -1) {
    int flagindex = this->flagoffset / 8;
    int offset = this->flagoffset % 8;
    char mask = 0x0F << offset;

    char newvalue = this->currentIndex();
    flags[flagindex] = (flags[flagindex] & (~mask)) | (newvalue << offset);
  }
}

bool LevelComboBox::updateState(const char *flags)
{
  if (this->flagoffset > -1) {
    int flagindex = this->flagoffset / 8;
    int iso = ((0x0F << (this->flagoffset % 8)) & flags[flagindex]) >> (this->flagoffset % 8);

    this->setCurrentIndex(iso);
  }
  return true;
}

ButtonEntry::ButtonEntry(QWidget *parent) :
        QWidget(parent)
{
    QVBoxLayout *vbox = new QVBoxLayout();
    QHBoxLayout *hbox = new QHBoxLayout();
    this->textBox = new QLineEdit(this);
    this->button = new QPushButton("Click", this);
    this->label = new QLabel("", this);
    vbox->addWidget(this->label);
    hbox->addWidget(this->textBox);
    hbox->addWidget(this->button);
    vbox->addLayout(hbox);
    this->setLayout(vbox);
    connect(this->button, SIGNAL(clicked()), this, SLOT(handleButton()));
    connect(this->textBox, SIGNAL(textEdited(QString)), this, SLOT(handleEdit(QString)));
}


QString ButtonEntry::text()
{
    return this->textBox->text();
}

void ButtonEntry::setText(QString text)
{
    this->textBox->setText(text);
}

FileEntry::FileEntry(QWidget *parent) : ButtonEntry(parent)
{
    this->button->setText("Browse...");
    this->label->setText("ROM File");
}

void FileEntry::handleButton()
{
    QString filename = QFileDialog::getOpenFileName(
           this, "Choose the ROM file", "./", "NES ROM Files (*.nes)");
    if (filename.size())
        this->textBox->setText(filename);
}

DirEntry::DirEntry(QWidget *parent) : ButtonEntry(parent)
{
    this->button->setText("Browse...");
#ifdef _WIN32
    this->label->setText("Output Folder");
#else
    this->label->setText("Output Directory");
#endif
}

void DirEntry::handleButton()
{
    QString dirName = QFileDialog::getExistingDirectory(this,
#ifdef _WIN32
            "Choose the output folder",
#else
            "Choose the output directory",
#endif
            "./");
    if (dirName.size())
        this->textBox->setText(dirName);
}

SeedEntry::SeedEntry(QWidget *parent) : ButtonEntry(parent)
{
    this->button->setText("Random");
    this->label->setText("Seed");
    this->handleButton();
}

void SeedEntry::random()
{
    uint64_t seed;
    char seedString[21];
    srand(time(NULL));
    seed = ((uint64_t)rand() << 32) | ((uint64_t)rand() & 0xffffffffL);
    mt_init(seed);
    seed = mt_rand64();
    snprintf(seedString, 21, "%" PRIu64, seed);
    this->textBox->setText(QString(seedString));
}

void SeedEntry::handleButton()
{
    this->random();
}

uint64_t SeedEntry::getSeed()
{
    uint64_t seed;
    sscanf(this->textBox->text().toLatin1().constData(), "%" PRIu64, &seed);
    return seed;
}

FlagEntry::FlagEntry(QWidget *parent) : ButtonEntry(parent)
{
    this->button->setText("Defaults");
    this->label->setText("Flags");
}

void FlagEntry::handleButton()
{
    this->setText(DEFAULT_FLAGS);
    this->textBox->textEdited(this->text());
}

void FlagEntry::handleEdit(QString text)
{
    this->textEdited(text);
}

