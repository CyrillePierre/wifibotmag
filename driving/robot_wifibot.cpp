#include "robot_wifibot.h"

Robot_Wifibot::Robot_Wifibot(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{	
	setWindowTitle(tr("Robot_Wifibot"));
    setMinimumSize(450, 600);
    resize(450, 600);

    setWindowIcon(QIcon(":/images/wifibot_vide.png"));

    QWidget *mainwidget = new QWidget;
    setCentralWidget(mainwidget);

    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	statusBar = new QStatusBar(this);
	this->setStatusBar(statusBar);

	abstractCommand = 0;

	createActions();
	createMenus();

	
	// Création du controller du robot.
	controler = new Controler;
	// Création de l'écran de la vidéo.
	interfaceVideo = new InterfaceVideo(mainwidget,controler);



	
	// Création des différents indicateurs.
	rightInfraredIndicator = new QProgressBar(this);
	rightInfraredIndicator->setOrientation(Qt::Vertical);
	leftInfraredIndicator = new QProgressBar(this);
	leftInfraredIndicator->setOrientation(Qt::Vertical);
	QHBoxLayout *videoLayout = new QHBoxLayout;
	videoLayout->addWidget(leftInfraredIndicator);
	videoLayout->addWidget(interfaceVideo);
	videoLayout->addWidget(rightInfraredIndicator);
	videoLayout->setSpacing(5);

	bateryLevel = new QProgressBar(this);
	bateryLevel->setTextVisible(true);
	bateryLevel->setMaximum(160);
	bateryLevel->setMinimum(0);
	bateryLevel->setValue(0);
	QHBoxLayout *bateryLayout = new QHBoxLayout;
	bateryLayout->addWidget(bateryLevel);
	QGroupBox *bateryGroup = new QGroupBox;
	bateryGroup->setTitle(tr("Etat de la baterie"));
	bateryGroup->setLayout(bateryLayout);

	speedWheelUpRight = new QLCDNumber(this);
	speedWheelUpLeft = new QLCDNumber(this);
	speedWheelDownRight = new QLCDNumber(this);
	speedWheelDownLeft = new QLCDNumber(this);
	QLabel *speedWheelUpRightLabel = new QLabel(tr("Avant droite"), this);
	QLabel *speedWheelUpLeftLabel = new QLabel(tr("Avant gauche"), this);
	QLabel *speedWheelDownRightLabel = new QLabel(tr("Arrière droite"), this);
	QLabel *speedWheelDownLeftLabel = new QLabel(tr("Arrière gauche"), this);
	QGridLayout *speedLayout = new QGridLayout;
	speedLayout->addWidget(speedWheelUpLeftLabel, 0, 0);
	speedLayout->addWidget(speedWheelUpRightLabel, 0, 2);
	speedLayout->addWidget(speedWheelUpLeft, 0, 1);
	speedLayout->addWidget(speedWheelUpRight, 0, 3);
	speedLayout->addWidget(speedWheelDownLeftLabel, 1, 0);
	speedLayout->addWidget(speedWheelDownRightLabel, 1, 2);
	speedLayout->addWidget(speedWheelDownLeft, 1, 1);
	speedLayout->addWidget(speedWheelDownRight, 1, 3);
	QGroupBox *speedGroup = new QGroupBox;
	speedGroup->setTitle(tr("Vitesse des roues"));
	speedGroup->setLayout(speedLayout);
	
	// Layout principal.
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(videoLayout);
	mainLayout->addWidget(bateryGroup);
	mainLayout->addWidget(speedGroup);
	mainwidget->setLayout(mainLayout);

	// Création du modèle de configuration.
	configModel = new ConfigModel(this);

	connectDialog = new ConnectDialog(configModel, controler, interfaceVideo, this);
	configDialog = new ConfigDialog(configModel, this);

	// Création des commandes.
	keyboard = new Keyboard(configModel, controler, this);
	joystick = new Joystick(configModel, controler, this);

	// Information a l'utilisateur.
	updateStatusBar(tr("Etat : Déconnecté"));
	
	// Signal emis par la boite de dialogue de connexion au Wifibot pour signaler que la connexion a réussie.
	connect(connectDialog, SIGNAL(isConnected(bool)), this, SLOT(Connect(bool)));
    connect(QuitterAct, SIGNAL(triggered()), this, SLOT(close()));
	connect(ConnectAct, SIGNAL(triggered()), connectDialog, SLOT(InitConnectDialog()));
    connect(DisconnectAct, SIGNAL(triggered()), this, SLOT(Disconnect()));
    connect(JoystickAct, SIGNAL(triggered()), this, SLOT(InJoystick()));
    connect(KeyboardAct, SIGNAL(triggered()), this, SLOT(InKeyboard()));
	connect(ConfigAct, SIGNAL(triggered()), configDialog, SLOT(InitConfigDialog()));
	connect(HowAct, SIGNAL(triggered()), this, SLOT(HowToDo()));
	connect(AboutAct, SIGNAL(triggered()), this, SLOT(About()));
	connect(AboutQtAct, SIGNAL(triggered()), this, SLOT(AboutQt()));

	// Connexion des parametres retourné par le contrôle robot.
	connect(controler,SIGNAL(batteryValue(int)),bateryLevel, SLOT(setValue(int))) ;
	connect(controler,SIGNAL(leftIR(int)),leftInfraredIndicator, SLOT(setValue(int)));
	connect(controler,SIGNAL(rightIR(int)),rightInfraredIndicator , SLOT(setValue(int)));
	connect(controler,SIGNAL(flWheelTicks(int)),speedWheelUpLeft, SLOT(display(int)));
	connect(controler,SIGNAL(frWheelTicks(int)),speedWheelUpRight, SLOT(display(int)));
	connect(controler,SIGNAL(rlWheelTicks(int)),speedWheelDownLeft, SLOT(display(int)));
	connect(controler,SIGNAL(rrWheelTicks(int)),speedWheelDownRight, SLOT(display(int)));
	
}

Robot_Wifibot::~Robot_Wifibot()
{
	if (keyboard)
		delete keyboard;
	if (joystick)
		delete joystick;
	if (interfaceVideo)
		delete interfaceVideo;
	if (controler)
		delete controler;
}

void Robot_Wifibot::createActions()
{

	QuitterAct = new QAction(tr("&Quitter"), this);
    QuitterAct->setShortcut(tr("Ctrl+Q"));
	QuitterAct->setStatusTip(tr("Quitter application"));

	ConnectAct = new QAction(tr("&Connexion"), this);
    ConnectAct->setShortcut(tr("Ctrl+C"));
	ConnectAct->setStatusTip(tr("Connexion Wifibot"));
 
	DisconnectAct = new QAction(tr("&Deconnexion"), this);
    DisconnectAct->setShortcut(tr("Ctrl+D"));
	DisconnectAct->setStatusTip(tr("Deconnexion Wifibot"));

	JoystickAct  = new QAction(tr("&Joystick"), this);
    JoystickAct->setShortcut(tr("Ctrl+J"));
	JoystickAct->setStatusTip(tr("Commande Joystick"));

	KeyboardAct  = new QAction(tr("&Keyboard"), this);
    KeyboardAct->setShortcut(tr("Ctrl+L"));
	KeyboardAct->setStatusTip(tr("Commande Clavier"));

	CommandGrp = new QActionGroup(this);
	CommandGrp->addAction(JoystickAct);
	CommandGrp->addAction(KeyboardAct);
	JoystickAct->setCheckable(true);
	KeyboardAct->setCheckable(true);

	ConfigAct  = new QAction(tr("&Configuration"), this);
    ConfigAct->setShortcut(tr("Ctrl+F"));
	ConfigAct->setStatusTip(tr("Configuration"));

	HowAct = new QAction(tr("&Aide"), this);
    HowAct->setShortcut(tr("Ctrl+A"));
	HowAct->setStatusTip(tr("Comment faire"));

	AboutAct = new QAction(tr("&About"), this);
	AboutAct->setStatusTip(tr("Version"));

	AboutQtAct = new QAction(tr("&AboutQt"), this);
	AboutQtAct->setStatusTip(tr("Version Qt"));

}

void Robot_Wifibot::createMenus()
{
	// Menu fichier.
	 fileMenu = menuBar()->addMenu(tr("&Fichier"));
     fileMenu->addAction(QuitterAct);
	 
	 // Menu wifibot.
	 wifibotMenu = menuBar()->addMenu(tr("&WiFiBot"));
	 wifibotMenu->addAction(ConnectAct);
	 wifibotMenu->addSeparator();
	 wifibotMenu->addAction(DisconnectAct);

	// Menu commandes.
	commandsMenu = menuBar()->addMenu(tr("&Commandes"));
	commandsMenu->addAction(JoystickAct);
	commandsMenu->addAction(KeyboardAct);
	commandsMenu->addSeparator();
	commandsMenu->addAction(ConfigAct);
	
   // Menu aide.
	aideMenu = menuBar()->addMenu(tr("&Aide"));
	aideMenu->addAction(HowAct);
	aideMenu->addSeparator();
	aideMenu->addAction(AboutAct);
	aideMenu->addAction(AboutQtAct);
}

void Robot_Wifibot::Connect(bool result)
{
	if (result)
	{
		ConnectAct->setEnabled(false);
		DisconnectAct->setEnabled(true);
		JoystickAct->setEnabled(true);
		KeyboardAct->setEnabled(true);
		
		// Information.
		updateStatusBar(tr("Etat : Connecté"));
	}
	else
	{
		ConnectAct->setEnabled(true);
		DisconnectAct->setEnabled(false);
		if (JoystickAct->isChecked())
			JoystickAct->setChecked(false);
		if (KeyboardAct->isChecked())
			KeyboardAct->setChecked(false);

		JoystickAct->setEnabled(false);
		KeyboardAct->setEnabled(false);

		// Information.
		updateStatusBar(tr("Etat : Déconnecté"));
	}

}

void Robot_Wifibot::Disconnect()
{
	controler->disconnect_robot();
	interfaceVideo->stop_video();
	if (this->abstractCommand)
		this->abstractCommand->stop();
	//Mise à jour de l'affichage.
	this->Connect(false);
	
}

void Robot_Wifibot::InJoystick()
{
	this->setCommand(joystick, tr("Joystick"));
}

void Robot_Wifibot::InKeyboard()
{
	this->setCommand(keyboard, tr("Clavier"));
}

void Robot_Wifibot::setCommand(AbstractCommand *abstractCommand, const QString &name)
{
	if (this->abstractCommand)
		this->abstractCommand->stop();
	updateStatusBar(tr("Commande : ")+name);
	this->abstractCommand = abstractCommand;
	this->abstractCommand->start();
}

void Robot_Wifibot::updateStatusBar(const QString& message)
{
	statusBar->showMessage(message, 5000);
}


void Robot_Wifibot::HowToDo()
{
     QMessageBox::about(this, tr("Menu About"),
             tr("Interface développée par R.AUFRERE 12/2012 "));

}

void Robot_Wifibot::AboutQt()
{
	QMessageBox::aboutQt(this, tr("About Qt"));
}

void Robot_Wifibot::About()
{
	QMessageBox::about(this,tr("About"),
		     tr("Interactive-Bot \n\n"
			 "Application permettant le contrôle d'un robot Wifibot et \n"
			 "la visualisation de la vidéo fournie par la camera DCS900 embarquée\n\n"));
}
/*
void Wifibot::onError(const QString &message)
{
	QMessageBox::information(this, tr("Erreur"), message);
}*/

