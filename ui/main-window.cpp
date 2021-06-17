
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStatusBar>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QIODevice>
#include <QtGui/QPalette>
#include <QtGui/QColor>
#include <QtGui/QGuiApplication>
#include <QtGui/QClipboard>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTabWidget>

#define __STDC_FORMAT_MACROS
#include <cinttypes>

#include "dwr.h"
#include "sprites.h"
#include "main-window.h"

#include "base64.h"

#define PANE_COLUMNS 2
#define PANE_ROWS    7

const char* const tab_names[] {
    "Gameplay",
    "Features",
    "Monsters",
    "Shortcuts",
    "Challenge",
    "Goals",
    "Cosmetic"
};

enum tabs {
    GAMEPLAY,
    FEATURES,
    MONSTERS,
    SHORTCUTS,
    CHALLENGE,
    GOALS,
    COSMETIC
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->new_flags = new char[FLAG_CHAR_LENGTH];
    this->setMinimumWidth(650);
    this->mainWidget = new QWidget();
    this->gameplayWidget = new QWidget();
    this->funWidget = new QWidget();
    this->setCentralWidget(this->mainWidget);

    this->initWidgets();
    this->initStatus();
    this->layout();
    this->initSlots();
    this->loadConfig();
}

void MainWindow::initStatus() {
    QStatusBar *status = this->statusBar();
    status->showMessage("Ready");
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, Qt::lightGray);
    palette.setColor(QPalette::Foreground, Qt::black);
    status->setPalette(palette);
    status->setAutoFillBackground(true);
}

void MainWindow::initWidgets()
{
    this->romFile = new FileEntry(this);
    this->outputDir = new DirEntry(this);
    this->seed = new SeedEntry(this);
    this->flags = new FlagEntry(this);
    this->levelSpeed = new LevelComboBox(52, this);
    this->goButton = new QPushButton("Randomize!", this);
    this->spriteSelect = new QComboBox(this);
    for (size_t i=0; i < sprite_count(); ++i) {
       spriteSelect->addItem(dwr_sprite_names[i]);
    }
    this->tabWidget = new QTabWidget(this);

    for (int i=0; i < TAB_COUNT; i++) {
      this->tabContents[i] = new QWidget(this);
    }
}

void MainWindow::initSlots()
{
    connect(this->flags, SIGNAL(textEdited(QString)), this, SLOT(handleFlags()));
    connect(this->levelSpeed, SIGNAL(activated(int)),
            this, SLOT(handleCheckBox()));
    connect(this->goButton,     SIGNAL(clicked()), this, SLOT(handleButton()));
}

void MainWindow::layout()
{
    QVBoxLayout *vbox;
    QGridLayout *grid;
    QGridLayout *goLayout;

    for (int i=0; i < TAB_COUNT; i++) {
      this->optionGrids[i] = new QGridLayout();
    }

    vbox = new QVBoxLayout();
    grid = new QGridLayout();
    goLayout = new QGridLayout();

    vbox->addLayout(grid);
    vbox->addWidget(tabWidget);
    vbox->addLayout(goLayout);

    grid->addWidget(this->romFile,   0, 0, 0);
    grid->addWidget(this->outputDir, 0, 1, 0);
    grid->addWidget(this->seed,      1, 0, 0);
    grid->addWidget(this->flags,     1, 1, 0);

    /* Add tab names and initialize layouts */
    for (int i=0; i < TAB_COUNT; i++) {
      this->tabContents[i]->setLayout(this->optionGrids[i]);
      tabWidget->addTab(tabContents[i], tab_names[i]);
      for (int c=0; c < PANE_COLUMNS; c++) {
        this->addLabel("", i, 0, c);
      }
      /* add empty labels for padding */
      for (int r=1; r < PANE_ROWS; r++) {
        this->addLabel("", i, r, 0);
      }
    }

    /* Gameplay Options */
    this->addOption("Shuffle Chests && Searches", GAMEPLAY,  0, 0, SHUFFLE_CHESTS_OFFSET);
    this->addOption("Random Growth",              GAMEPLAY,  1, 0, RANDOMIZE_GROWTH_OFFSET);
    this->addOption("Random Map",                 GAMEPLAY,  2, 0, RANDOM_MAP_OFFSET);
    this->addOption("Random Spell Learning",      GAMEPLAY,  3, 0, RANDOMIZE_SPELLS_OFFSET);
    this->addOption("Heal/Hurt before \"More\"",  GAMEPLAY,  4, 0, HEAL_HURT_B4_MORE_OFFSET);
    this->addOption("Random Weapon Shops",        GAMEPLAY,  0, 1, RANDOMIZE_SHOPS_OFFSET);
    this->addOption("Random Weapon Prices",       GAMEPLAY,  1, 1, RANDOM_PRICES_OFFSET);
    this->addOption("Random XP Requirements",     GAMEPLAY,  2, 1, RANDOM_XP_REQS_OFFSET);

    /* Features */
    this->addOption("Enable Menu Wrapping",       FEATURES,  0, 0, MENU_WRAP_OFFSET);
    this->addOption("Enable Death Necklace",      FEATURES,  1, 0, DEATH_NECKLACE_OFFSET);
    this->addOption("Enable Torches In Battle",   FEATURES,  2, 0, TORCH_IN_BATTLE_OFFSET);
    this->addOption("Big Swamp",                  FEATURES,  0, 1, BIG_SWAMP_OFFSET);
    this->addOption("Repel in dungeons",          FEATURES,  1, 1, REPEL_IN_DUNGEONS_OFFSET);
    this->addOption("Permanent repel",            FEATURES,  2, 1, PERMANENT_REPEL_OFFSET);
    this->addOption("Permanent torch",            FEATURES,  3, 1, PERMANENT_TORCH_OFFSET);

    /* Monster flags */
    this->addOption("Random Monster Abilities",   MONSTERS,  0, 0, RANDOMIZE_PATTERNS_OFFSET);
    this->addOption("Random Monster Zones",       MONSTERS,  1, 0, RANDOMIZE_ZONES_OFFSET);
    this->addOption("Random Monster Stats",       MONSTERS,  2, 0, RANDOM_ENEMY_STATS_OFFSET);
    this->addOption("Random Monster XP && Gold",  MONSTERS,  3, 0, RANDOM_ENEMY_DROPS_OFFSET);
    this->addOption("Make Stats Consistent",      MONSTERS,  4, 0, CONSISTENT_STATS_OFFSET);
    this->addOption("Scared Metal Slimes",        MONSTERS,  0, 1, SCARED_SLIMES_OFFSET);

    /* Shortcuts */
    this->addOption("Fast Text",                  SHORTCUTS, 0, 0, FAST_TEXT_OFFSET);
    this->addOption("Speed Hacks",                SHORTCUTS, 1, 0, SPEED_HACKS_OFFSET);
    this->addOption("Open Charlock",              SHORTCUTS, 2, 0, OPEN_CHARLOCK_OFFSET);
    this->addOption("Short Charlock",             SHORTCUTS, 3, 0, SHORT_CHARLOCK_OFFSET);
    this->addOption("Don't Require Magic Keys",   SHORTCUTS, 4, 0, NO_KEYS_OFFSET);
    this->addLabel("Leveling Speed",              SHORTCUTS, 0, 1);
    this->placeWidget(this->levelSpeed,           SHORTCUTS, 1, 1); // bits 52-55 

    /* Challenge */
    this->addOption("No Hurtmore",                CHALLENGE, 0, 0, NO_HURTMORE_OFFSET);
    this->addOption("No Numbers",                 CHALLENGE, 1, 0, NO_NUMBERS_OFFSET);
    this->addOption("Invisible Hero",             CHALLENGE, 2, 0, INVISIBLE_HERO_OFFSET);
    this->addOption("Invisible NPCs",             CHALLENGE, 3, 0, INVISIBLE_NPCS_OFFSET);

    /* Goals */
    this->addOption("Cursed Princess",            GOALS,     0, 0, CURSED_PRINCESS_OFFSET);
    this->addOption("Three's Company",            GOALS,     1, 0, THREES_COMPANY_OFFSET);

    /* Cosmetic Options */
    this->addOption("Shuffle Music",              COSMETIC,  0, 0, RANDOMIZE_MUSIC_OFFSET);
    this->addOption("Disable Music",              COSMETIC,  1, 0, DISABLE_MUSIC_OFFSET);
    this->addOption("Modern Spell Names",         COSMETIC,  2, 0, MODERN_SPELLS_OFFSET);
    this->addOption("Noir Mode",                  COSMETIC,  3, 0, NOIR_MODE_OFFSET);
    this->addOption("Disable Spell Flashing",     COSMETIC,  4, 0, NO_SCREEN_FLASH_OFFSET);
    this->addLabel("Player Sprite",               COSMETIC,  0, 1);
    this->placeWidget(this->spriteSelect,         COSMETIC,  1, 1);  // Not in flags

    goLayout->addWidget(new QLabel("", this), 0, 0, 0);
    goLayout->addWidget(new QLabel("", this), 0, 1, 0);
    goLayout->addWidget(this->goButton, 0, 2, 0);

    this->mainWidget->setLayout(vbox);
}

void MainWindow::addOption(QString text, int tab, int x, int y, int offset)
{
    CheckBox *option = new CheckBox(text, QString(), QString(), this, offset);
    option->setTristate(true);
    connect(option, SIGNAL(clicked()), this, SLOT(handleCheckBox()));
    this->options.append(option);
    this->optionGrids[tab]->addWidget(option, x, y, 0);
}

void MainWindow::addOption(QString text, QString requires,
        QString excluded_by, int tab, int x, int y, int flagoffset)
{
    CheckBox *option = new CheckBox(text, requires, excluded_by, this, flagoffset);
    option->setTristate(true);
    connect(option, SIGNAL(clicked()), this, SLOT(handleCheckBox()));
    this->options.append(option);
    this->optionGrids[tab]->addWidget(option, x, y, 0);
}

void MainWindow::addLabel(QString text, int tab, int x, int y)
{
    this->optionGrids[tab]->addWidget(new QLabel(text, this), x, y, 0);
}

void MainWindow::placeWidget(QWidget *widget, int tab, int x, int y)
{
    this->optionGrids[tab]->addWidget(widget, x, y, 0);
}

QString MainWindow::getOptions()
{
    QList<CheckBox*>::const_iterator i;
    bool conflicts = false;

    for (i = this->options.begin(); i != this->options.end(); ++i) {
        (*i)->writeFlag(new_flags);
    }

    /* check for flag conflicts */
    for (i = this->options.begin(); i != this->options.end(); ++i) {
        conflicts |= (*i)->updateConflicts(new_flags);
    }

    if (conflicts) { /* there were conflicts, so update flags */
        this->levelSpeed->writeFlag(new_flags);
        for (i = this->options.begin(); i != this->options.end(); ++i) {
            (*i)->writeFlag(new_flags);
        }
    }

    char output[20];
    bscrypt_base64_encode(output, new_flags, FLAG_CHAR_LENGTH); 
    return QString(output);
}

void MainWindow::setOptions(QString flags)
{
    QList<CheckBox*>::const_iterator i;

    char temp_flags[FLAG_CHAR_LENGTH];
    QByteArray text = flags.toLocal8Bit();
    char *data = new char[text.size() + 1];
    strcpy(data, text.data());
    bscrypt_base64_decode(temp_flags, data, text.size() + 1);

    for (i = this->options.begin(); i != this->options.end(); ++i) {
        if ((*i)->updateConflicts(temp_flags)) {
            (*i)->updateState(temp_flags);
        }
    }

    this->levelSpeed->updateState(temp_flags);
    memcpy(new_flags, temp_flags, FLAG_CHAR_LENGTH);
    this->setFlags(this->getOptions());
    delete [] data;
}

QString MainWindow::getFlags()
{
    std::string flags = this->flags->text().toStdString();
    return QString::fromStdString(flags);
}

void MainWindow::setFlags(QString flags)
{  
    this->flags->setText(flags);
}

void MainWindow::handleCheckBox()
{
    QString flags = this->getOptions();
    this->setFlags(flags);
}

void MainWindow::handleComboBox(int index)
{
    this->handleCheckBox();
}

void MainWindow::handleFlags()
{
    QString flags = this->getFlags();
    this->setOptions(flags);
}

void MainWindow::handleButton()
{
    char checksum[64];

    uint64_t seed = this->seed->getSeed();
    std::string inputFile = this->romFile->text().toLatin1().constData();
    std::string outputDir = this->outputDir->text().toLatin1().constData();
    std::string spriteName =
                this->spriteSelect->currentText().toLatin1().constData();
    uint64_t crc = dwr_randomize(inputFile.c_str(), seed, this->new_flags,
                                 spriteName.c_str(), outputDir.c_str());
    if (crc) {
        sprintf(checksum, "Checksum: %016" PRIx64, crc);
        QGuiApplication::clipboard()->setText(checksum);
        this->statusBar()->showMessage(
                QString("%1 (copied to clipboard)").arg(checksum));
        QMessageBox::information(this, "Success!",
                                 "The new ROM has been created.");
    } else {
        QMessageBox::critical(this, "Failed", "An error occurred and"
                "the ROM could not be created.");
    }
    this->saveConfig();
}

bool MainWindow::saveConfig()
{
    QDir configDir("");
    if (!configDir.exists(QDir::homePath() + "/.config/")){
        configDir.mkdir(QDir::homePath() + "/.config/");
    }

    QFile configFile(QDir::homePath() + "/.config/dwrandomizer2.conf");
    if (!configFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        printf("Failed to save configuration.\n");
        return false;
    }
    QTextStream out(&configFile);

    out << this->romFile->text() << endl;
    out << this->outputDir->text() << endl;
    out << this->getFlags() << endl;
    out << this->spriteSelect->currentIndex() << endl;

    return true;
}

bool MainWindow::loadConfig()
{
    char tmp[1024];
    qint64 read;

    QFile configFile(QDir::homePath() + "/.config/dwrandomizer2.conf");
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        printf("Failed to load configuration.\n");
        return false;
    }
    read = configFile.readLine(tmp, 1024);
    if (read) {
        tmp[read - 1] = '\0';
        this->romFile->setText(tmp);
        if (configFile.atEnd()) {
            return false;
        }
    }

    read = configFile.readLine(tmp, 1024);
    if (read) {
        tmp[read - 1] = '\0';
        this->outputDir->setText(tmp);
        if (configFile.atEnd()) {
            return false;
        }
    }

    read = configFile.readLine(tmp, 1024);
    if (read) {
        tmp[read - 1] = '\0';
        this->setFlags(tmp);
        this->setOptions(tmp);
    }

    read = configFile.readLine(tmp, 1024);
    if (read) {
        int spriteIndex = atoi(tmp);
        this->spriteSelect->setCurrentIndex(spriteIndex);
    }

    return true;
}
