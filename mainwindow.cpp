#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->dateEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    ui->dateEdit->setDateTime(QDateTime::fromString(srandDateTime(),"yyyy-MM-dd hh:mm:ss"));
    ui->dateEdit->setCalendarPopup(true);

    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(18, 118, 116));
    setAutoFillBackground(true);
    setPalette(pal);
//    ui->lineEdit->setFocus();//不能用？？？很神奇
    QTimer::singleShot(0, ui->lineEdit, SLOT(setFocus()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::createRegistrationCode()
{
    QString machineCode = ui->lineEdit->text();
    QString macAddress = machineCode.replace("-","");
    macAddress = macAddress.mid(0,2)+macAddress.mid(4,2)+macAddress.mid(8,2)+macAddress.mid(12,2)+macAddress.mid(16,2)+macAddress.mid(20,2);

    return macAddress;
}

QString MainWindow::getCPUId()
{
    QString machineCode = ui->lineEdit->text();
    QString cpuId = machineCode.replace("-","");
    cpuId = cpuId.mid(2,2)+cpuId.mid(6,2)+cpuId.mid(10,2)+cpuId.mid(14,2)+cpuId.mid(18,2)+cpuId.mid(22);

    return cpuId;
}

QString MainWindow::eversedOrder(QString str)
{
    for(int i = str.size()-1;i>=0;i--)
    {
        str+=str.mid(i,1);
    }
    str = str.mid(str.size()/2);
    return str;
}

QString MainWindow::srandDateTime()
{
    int yer,mon,day,hh,mm,ss;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    yer=2090+qrand()%9;
    mon=1+qrand()%11;
    day=1+qrand()%29;
    hh=qrand()%23;
    mm=qrand()%59;
    ss=qrand()%59;
    QString m_mon=mon<10? QString("0%1").arg(QString::number(mon)) : QString::number(mon);
    QString m_day=day<10? QString("0%1").arg(QString::number(day)) : QString::number(day);
    QString m_hh=hh<10? QString("0%1").arg(QString::number(hh)) : QString::number(hh);
    QString m_mm=mm<10? QString("0%1").arg(QString::number(mm)) : QString::number(mm);
    QString m_ss=ss<10? QString("0%1").arg(QString::number(ss)) : QString::number(ss);
    //qDebug()<<QString::number(yer)+"-"+m_mon+"-"+m_day+" "+m_hh+":"+m_mm+":"+m_ss;
    return QString::number(yer)+"-"+m_mon+"-"+m_day+" "+m_hh+":"+m_mm+":"+m_ss;
}

bool MainWindow::isMacAddress(QString mac)
{
    QRegExp rx("^([A-Fa-f0-9]{2}){5}[A-Fa-f0-9]{2}$");//^([A-Fa-f0-9]{2}[-,:]){5}[A-Fa-f0-9]{2}$
    QRegExpValidator v(rx, nullptr);
    int pos = 0;
    if(v.validate(mac, pos) == QValidator::Acceptable)
        return true;
    else
        return false;
}

void MainWindow::writeToLog()
{
    QDateTime time=QDateTime::currentDateTime();
    QString tmpStr1=QStringLiteral("[") + time.toString("yyyy-MM-dd hh:mm:ss") + QStringLiteral("] 机器码:") + ui->lineEdit->text() + "\r\n";
    QString tmpStr2=QStringLiteral("[") + time.toString("yyyy-MM-dd hh:mm:ss") + QStringLiteral("] 注册码:") + ui->lineEdit_2->text() + "\r\n";
    QString tmpStr3=QStringLiteral("[") + time.toString("yyyy-MM-dd hh:mm:ss") + QStringLiteral("] 有效期:") + ui->dateEdit->text() + "\r\n";
    QString tmpStr4="----------------------------------------------------------------\r\n";
    QString fullPath="D:/RegisterCode.log";
    QFile logData(fullPath);
    if(logData.open(QIODevice::WriteOnly | QIODevice::Append)){
        QTextStream in(&logData);
        in.setCodec("UTF-8");
        in << tmpStr1;
        in << tmpStr2;
        in << tmpStr3;
        in << tmpStr4;
        logData.close();
    }
}

void MainWindow::on_pushButton_clicked()
{

    QString macAddress = createRegistrationCode();
    if(isMacAddress(macAddress) == false)
    {
        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("无效的机器码"), QMessageBox::Ok);
        return;
    }
    macAddress = eversedOrder(macAddress);
    QString time = eversedOrder(ui->dateEdit->dateTime().toString("yyyyMMddhhmmss").mid(2));
    QString RegistrationCode = macAddress.mid(0,2)+time.mid(0,2)+"-"+macAddress.mid(2,2)+time.mid(2,2)+"-"
            +macAddress.mid(4,2)+time.mid(4,2)+"-"+macAddress.mid(6,2)+time.mid(6,2)+"-"
            +macAddress.mid(8,2)+time.mid(8,2)+"-"+macAddress.mid(10,2)+time.mid(10,2);
    ccXor ccx;
    QString realCode=ccx.XorEncryptDecrypt(RegistrationCode,1);
    realCode.replace(ccx.XorEncryptDecrypt("-",1),"-");
    ui->lineEdit_2->setText(realCode);
#ifdef _WIN32
    writeToLog();
#endif
    ui->dateEdit->setDateTime(QDateTime::fromString(srandDateTime(),"yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::on_pushButton_2_clicked()
{
    qApp->exit(0);
}
