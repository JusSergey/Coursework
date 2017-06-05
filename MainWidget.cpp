#include "MainWidget.h"
#include "SettingEffect.h"
#include "Effects.h"

constexpr auto FOR_PLAYER_SOURCE = "ForPlayer.wav";

constexpr auto MENU_FILE = "Файл";
constexpr auto OPEN  = "Відкрити";
constexpr auto CLOSE = "Закрити";
constexpr auto SAVE  = "Зберегти";
constexpr auto SAVE_AS = "Зберегти як...";
constexpr auto EDITOR = "Редактор";
constexpr auto PLAYER = "Плеєр";


constexpr auto NORMALIZE = "Нормалізація";
constexpr auto FADE_OUT  = "Затухання";
constexpr auto FADE_IN   = "Наростання";
constexpr auto REVERS_Y  = "Реверс Y";
constexpr auto REVERS_X  = "Реверс X";

constexpr auto NOISE_LOG = "Шум";
constexpr auto SINOSYDE  = "Синус";
constexpr auto CUT       = "Вирізати";
constexpr auto CROP      = "Обрізати";

constexpr auto FORCE = "Сила";
constexpr auto HZ = "Частота";
constexpr auto AMPLITUDE = "Амплітуда";

QString getPlayerFilePath() {
    return QApplication::applicationDirPath() + FOR_PLAYER_SOURCE;
}

MainWidget::MainWidget(QWidget *parent)
    : QMainWindow(parent),
      readfilesize(0),
      player(new PlayerWindow)
{
    initMenuBar();

    directorView = new DirectorView(this);
    drawView = new DrawWidget(this);

    QVBoxLayout *l = new QVBoxLayout;

    l->addWidget(drawView);
    l->addWidget(directorView);

    QWidget *w = new QWidget(this);
    w->setLayout(l);

    setCentralWidget(w);

    decoder = new QAudioDecoder(this);
    connect(decoder, SIGNAL(bufferReady()), this, SLOT(slotDecoderBufferReady()), Qt::UniqueConnection);
    connect(decoder, SIGNAL(finished()), this, SLOT(slotAudioDecodeFinish()), Qt::UniqueConnection);

    connect(directorView, SIGNAL(signalChangeScaleW(float)), this, SLOT(slotChangeScaleW(float)));
    connect(directorView, SIGNAL(signalChangeScaleH(float)), this, SLOT(slotChangeScaleH(float)));

    setMinimumSize(500, 400);

    setWindowTitle("Курсова робота студента групи 2-КТ-14 Джуса С. В.");

    drawView->setScaleW(directorView->getScaleW());

    setEnableAction(EDITOR, false);
    setEnableAction(SAVE, false);
    setEnableAction(SAVE_AS, false);
    setEnableAction(CLOSE, false);
    setEnableAction(PLAYER, false);
    installEventFilter(this);


}

MainWidget::~MainWidget()
{

}

bool MainWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {

        QKeyEvent *e = static_cast<QKeyEvent *> (event);

        if (e->key() == Qt::Key_Space) {
            slotPlayer();
            return true;
        }
    }
    return false;
}

void MainWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
    qDebug()<< "enter drag";
}


void MainWidget::dropEvent(QDropEvent *event)
{
    qDebug()<< "drop event";
    auto urls = event->mimeData()->urls();
    if (urls.size() != 1) {
        QMessageBox::information(this, "Помилка", "Перетягніть тільки один файл");
        return;
    }
    else {
        openFile(urls.first().toLocalFile());
    }
}

void MainWidget::slotButtonSelectFile()
{
}

void MainWidget::slotButtonOpenDialogDelectFile()
{
}

void MainWidget::slotDecoderBufferReady()
{
    auto buffer = decoder->read();
    if (buffer.format().channelCount() != 2 || buffer.format().sampleSize() != 2) {
        qDebug() << buffer.format().channelCount() << buffer.format().sampleSize() ;
        QMessageBox::information(nullptr, "error", "");
        decoder->blockSignals(true);
        decoder->stop();
    }
    readfilesize += (buffer.byteCount() / buffer.format().channelCount() / (buffer.format().sampleSize() / 8));
    readfile.push_back(std::move(buffer));
}

void MainWidget::slotAudioDecodeFinish()
{
    qDebug() << "MainWidget::slotAudioDecodeFinish(): ";
    qDebug() << readfile.front().format().sampleSize();

    setEnableAction(EDITOR, true);
    setEnableAction(SAVE, true);
    setEnableAction(SAVE_AS, true);
    setEnableAction(CLOSE, true);
    setEnableAction(PLAYER, true);

    drawView->setAudioBufferList(readfile);
}

void MainWidget::slotOpen() {
    qDebug() << OPEN;

    QString filepath = QFileDialog::getOpenFileName(this, "", "", "Audio Files (*.wav *.mp3)");

    if (!QFileInfo(filepath).isFile()) {
        qDebug() << "can't open file: " << filepath;
        return;
    }
    openFile(filepath);
}

void MainWidget::slotSave() {

    qDebug() << SAVE;
    saveFile(fpath);
}

void MainWidget::slotSaveAs()
{
    qDebug() << SAVE_AS;

    QString filepath = QFileDialog::getSaveFileName(this, "", "", "Audio Files (*.wav)");

    if (filepath.isEmpty())
        return;

    saveFile(filepath);

}

void MainWidget::slotClose() {
    qDebug() << CLOSE;
    readfilesize = 0;
    drawView->resetBuffer();
    drawView->refresh();
    setEnableAction(EDITOR, false);
    setEnableAction(SAVE, false);
    setEnableAction(SAVE_AS, false);
    setEnableAction(CLOSE, false);
    setEnableAction(PLAYER, false);
}

void MainWidget::slotPlayer()
{
    saveFile(getPlayerFilePath());
    player->setSource(getPlayerFilePath());
    player->show();
}

void MainWidget::slotChangeScaleW(float scale)
{
    drawView->setScaleW(scale);
}

void MainWidget::slotChangeScaleH(float scale)
{
    drawView->setScaleH(scale);
}

/// CLICKED ACIONS
void MainWidget::slotNormalize()
{
    qDebug() << NORMALIZE;

    auto setting = new SettingEffect({AMPLITUDE}, nullptr);
    setting->show();

    connect(setting, SIGNAL(signalClickedButtonOk(bool, std::map<QString, float>)),
            this,    SLOT(slotAcceptNormalize(bool, std::map<QString, float>)));
}

void MainWidget::slotFadeIn()
{
    qDebug() << FADE_IN;

    auto setting = new SettingEffect({FORCE, AMPLITUDE}, nullptr);
    setting->show();

    connect(setting, SIGNAL(signalClickedButtonOk(bool, std::map<QString, float>)),
            this,    SLOT(slotAcceptFadeIn(bool, std::map<QString, float>)));
}

void MainWidget::slotFadeOut()
{
    qDebug() << FADE_OUT;

    auto setting = new SettingEffect({FORCE, AMPLITUDE}, nullptr);
    setting->show();

    connect(setting, SIGNAL(signalClickedButtonOk(bool, std::map<QString, float>)),
            this,    SLOT(slotAcceptFadeOut(bool, std::map<QString, float>)));
}

void MainWidget::slotReversY()
{
    qDebug() << REVERS_Y;

    auto setting = new SettingEffect({}, nullptr);
    setting->show();

    connect(setting, SIGNAL(signalClickedButtonOk(bool, std::map<QString, float>)),
            this,    SLOT(slotAcceptReversY(bool,std::map<QString, float>)));
}

void MainWidget::slotReversX()
{
    auto setting = new SettingEffect({}, nullptr);
    setting->show();

    connect(setting, SIGNAL(signalClickedButtonOk(bool, std::map<QString, float>)),
            this,    SLOT(slotAcceptReversX(bool,std::map<QString, float>)));
}

void MainWidget::slotNoiseLog()
{
    qDebug() << NOISE_LOG;

    auto setting = new SettingEffect({FORCE}, nullptr);
    setting->show();

    connect(setting, SIGNAL(signalClickedButtonOk(bool, std::map<QString, float>)),
            this,    SLOT(slotAcceptNoiseLog(bool, std::map<QString, float>)));
}

void MainWidget::slotSinosyde()
{
    qDebug() << SINOSYDE;

    auto setting = new SettingEffect({FORCE, AMPLITUDE, HZ}, nullptr);
    setting->show();

    connect(setting, SIGNAL(signalClickedButtonOk(bool, std::map<QString, float>)),
            this,    SLOT(slotAcceptSinosyde(bool, std::map<QString, float>)));
}

void MainWidget::slotCut()
{
    qDebug() << CUT;

    Effects::Cut(drawView->getBuffer(), drawView->getStartSelect(), drawView->getEndSelect());

    drawView->refresh();
}

void MainWidget::slotCrop()
{
    qDebug() << CROP;

    Effects::Crop(drawView->getBuffer(), drawView->getStartSelect(), drawView->getEndSelect());
    drawView->refresh();
}

/// ACCEPTS
void MainWidget::slotAcceptNormalize(bool isAllFile, std::map<QString, float> values)
{
    Effects::Normalize(drawView->getBuffer(),
                       values[AMPLITUDE],
                       isAllFile ? -1 : drawView->getStartSelect(),
                       isAllFile ? -1 : drawView->getEndSelect());

    drawView->refresh();
}

void MainWidget::slotAcceptFadeIn(bool isAllFile, std::map<QString, float> values)
{
    Effects::FadeIn(drawView->getBuffer(),
                    values[FORCE],
                    values[AMPLITUDE],
                    isAllFile ? -1 : drawView->getStartSelect(),
                    isAllFile ? -1 : drawView->getEndSelect(),
                    isAllFile ? readfilesize : -1);


    drawView->refresh();
}

void MainWidget::slotAcceptFadeOut(bool isAllFile, std::map<QString, float> values)
{
    Effects::FadeOut(drawView->getBuffer(),
                     values[FORCE],
                     values[AMPLITUDE],
                     isAllFile ? -1 : drawView->getStartSelect(),
                     isAllFile ? -1 : drawView->getEndSelect(),
                     isAllFile ? readfilesize : -1);

    drawView->refresh();
}

void MainWidget::slotAcceptReversY(bool isAllFile, std::map<QString, float> values)
{
    Q_UNUSED(values)

    Effects::ReverseY(drawView->getBuffer(),
                      isAllFile ? -1 : drawView->getStartSelect(),
                      isAllFile ? -1 : drawView->getEndSelect());

    drawView->refresh();
}

void MainWidget::slotAcceptReversX(bool isAllFile, std::map<QString, float> values)
{
    Q_UNUSED(values)

    Effects::ReverseX(drawView->getBuffer(),
                      isAllFile ? -1 : drawView->getStartSelect(),
                      isAllFile ? -1 : drawView->getEndSelect());

    drawView->refresh();

}

void MainWidget::slotAcceptNoiseLog(bool isAllFile, std::map<QString, float> values)
{
    Effects::NoiseLogarithmic(drawView->getBuffer(),
                              values[FORCE],
                              isAllFile ? -1 : drawView->getStartSelect(),
                              isAllFile ? -1 : drawView->getEndSelect());

    drawView->refresh();

}

void MainWidget::slotAcceptSinosyde(bool isAllFile, std::map<QString, float> values)
{
    Effects::Sinosyde(drawView->getBuffer(),
                      values[FORCE],
                      values[AMPLITUDE],
                      values[HZ],
                      isAllFile ? -1 : drawView->getStartSelect(),
                      isAllFile ? -1 : drawView->getEndSelect());

    drawView->refresh();
}

void MainWidget::saveForPlayer()
{
    saveFile(FOR_PLAYER_SOURCE);
}

void MainWidget::openFile(QString path)
{
    fpath = path;

    readfile.clear();

    decoder->setSourceFilename(fpath);
    decoder->start();
}

void MainWidget::saveFile(QString path)
{
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, "Помилка", "Неможливо відкрити файл для запису.");
        return;
    }

    size_t szfile = 0;

    file.seek(sizeof(WaveHead));

    for (const auto simpleBuffer : drawView->getBuffer()) {
        szfile += simpleBuffer.byteCount();
        file.write(static_cast<const char *>(simpleBuffer.data()), simpleBuffer.byteCount());
    }

    file.reset();

    const QAudioFormat format = drawView->getBuffer().front().format();

    WaveHead head;
    strncpy(head.chunkId, "RIFF", head.lenstr);
    head.chunkSize = szfile + sizeof(WaveHead) - sizeof(head.chunkSize) - sizeof(head.chunkId);
    strncpy(head.format, "WAVE", head.lenstr);
    strncpy(head.subchunk1Id, "fmt ", head.lenstr);
    head.subchunk1Size = 16; // for PCM;
    head.audioFormat = 1; // linear quantum
    head.numChannels = format.channelCount();
    head.sampleRate = format.sampleRate();
    head.byteRate = format.sampleRate() * format.channelCount() * format.sampleSize() / 8;
    head.blockAlign = format.channelCount() * format.sampleSize() / 8;
    head.bitsPerSample = format.sampleSize();
    strncpy(head.subchunk2Id, "data", head.lenstr);
    head.subchunk2Size = szfile;

    file.write((const char *)&head, sizeof(head));

    file.flush();
    file.close();
}

void MainWidget::initMenuBar()
{
    QMenu *menuFile = new QMenu(MENU_FILE, this);
    auto actOpen  = new QAction(OPEN, this);
    auto actClose = new QAction(CLOSE,this);
    auto actSave  = new QAction(SAVE, this);
    auto actSaveAs= new QAction(SAVE_AS, this);

    mActions = QList<QAction *>{actOpen, actClose, actSave, actSaveAs};

    menuFile->addActions(mActions);


    QMenu *editorMenu = new QMenu(EDITOR, this);
    auto actNormalize = new QAction(NORMALIZE, this);
    auto actFadeIn    = new QAction(FADE_IN, this);
    auto actFadeOut   = new QAction(FADE_OUT, this);
    auto actReversX   = new QAction(REVERS_X, this);
    auto actReversY   = new QAction(REVERS_Y, this);
    auto actNoiseLog  = new QAction(NOISE_LOG, this);
    auto actSinosyde  = new QAction(SINOSYDE, this);
    auto actCut       = new QAction(CUT, this);
    auto actCrop      = new QAction(CROP, this);

    editorMenu->addActions({actNormalize,
                            actFadeIn,
                            actFadeOut,
                            actReversX,
                            actReversY,
                            actNoiseLog,
                            actSinosyde,
                            actCut,
                            actCrop});

    auto actPlayer = new QAction(PLAYER, this);

    menu = new QMenuBar(this);
    menu->addMenu(menuFile);
    menu->addAction(actPlayer);
    menu->addMenu(editorMenu);

    connect(actNormalize, SIGNAL(triggered(bool)), this, SLOT(slotNormalize()));
    connect(actFadeIn,    SIGNAL(triggered(bool)), this, SLOT(slotFadeIn()));
    connect(actFadeOut,   SIGNAL(triggered(bool)), this, SLOT(slotFadeOut()));
    connect(actReversX,   SIGNAL(triggered(bool)), this, SLOT(slotReversX()));
    connect(actReversY,   SIGNAL(triggered(bool)), this, SLOT(slotReversY()));
    connect(actNoiseLog,  SIGNAL(triggered(bool)), this, SLOT(slotNoiseLog()));
    connect(actSinosyde,  SIGNAL(triggered(bool)), this, SLOT(slotSinosyde()));
    connect(actCut,       SIGNAL(triggered(bool)), this, SLOT(slotCut()));
    connect(actCrop,      SIGNAL(triggered(bool)), this, SLOT(slotCrop()));


    connect(actOpen,   SIGNAL(triggered(bool)), this, SLOT(slotOpen()));
    connect(actClose,  SIGNAL(triggered(bool)), this, SLOT(slotClose()));
    connect(actSave,   SIGNAL(triggered(bool)), this, SLOT(slotSave()));
    connect(actSaveAs, SIGNAL(triggered(bool)), this, SLOT(slotSaveAs()));
    connect(actPlayer, SIGNAL(triggered(bool)), this, SLOT(slotPlayer()));

    setMenuBar(menu);
}

void MainWidget::setEnableAction(QString action, bool enable)
{
    for (auto lists : {menu->actions(), mActions}){
        for(QAction *act : lists) {
            if (act->text() == action) {
                act->setEnabled(enable);
                break;
            }
        }
    }
}
