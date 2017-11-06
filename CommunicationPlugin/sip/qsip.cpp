#include "qsip.h"
#include <QDebug>
#include <QThread>
#define THIS_FILE	"APP"

simple_pjsua::simple_pjsua(QObject *parent) : QObject(parent)
{

    pThis = this;


}

simple_pjsua * simple_pjsua::pThis = NULL;
int simple_pjsua::my_pjsua_call_id = 0;


simple_pjsua::~simple_pjsua()
{
    qDebug()<<"delete simple_pjsua"<<QThread::currentThread()->objectName();
    pjsua_destroy();
}

int simple_pjsua::SipInit()
{
    status = Create();
    if(status == PJ_SUCCESS)
    {
        qDebug()<<status<<"simple_pjsua, Create";
        status = Init();
    }
    if(status == PJ_SUCCESS)
    {
        qDebug()<<status<<"simple_pjsua, Init";
        status = AddUDP();
    }
    if(status == PJ_SUCCESS)
    {
        qDebug()<<status<<"simple_pjsua, status";
        status = StartPjsua();
    }
    all_device();
    return status;
}

/* Create pjsua first! */
int simple_pjsua::Create()
{
    status = pjsua_create();
    if (status != PJ_SUCCESS)
        error_exit("Error in pjsua_create()", status);
    return status;
}

/* Init pjsua */
int simple_pjsua::Init()
{
    pjsua_config cfg;
    pjsua_logging_config log_cfg;

    pjsua_config_default(&cfg);
    cfg.cb.on_incoming_call = &simple_pjsua::on_incoming_call;
    cfg.cb.on_call_media_state = &simple_pjsua::on_call_media_state;
    cfg.cb.on_call_state = &simple_pjsua::on_call_state;

    pjsua_logging_config_default(&log_cfg);
    log_cfg.console_level = 4;

    status = pjsua_init(&cfg, &log_cfg, NULL);
    if (status != PJ_SUCCESS)
        error_exit("Error in pjsua_init()", status);
    return status;
}

/* Add UDP transport. */
int simple_pjsua::AddUDP()
{
    pjsua_transport_config cfg;

    pjsua_transport_config_default(&cfg);
    //cfg.port = 5060;
    cfg.port = 5061;
    status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, NULL);
    if (status != PJ_SUCCESS)
        error_exit("Error creating transport", status);
    return status;
}

/* Initialization is done, now start pjsua */
int simple_pjsua::StartPjsua()
{
    status = pjsua_start();
    if (status != PJ_SUCCESS)
        error_exit("Error starting pjsua", status);
    return status;
}

/* Register to SIP server by creating SIP account. */
int simple_pjsua::RegisterSIP(QString sip_user,QString password,QString sip_domain)
{
    pjsua_acc_config cfg;
    QString sip_full_id ="sip:"+sip_user+"@"+sip_domain;
    QString fulldomain = "sip:"+sip_domain;

    QByteArray fullIDflag = sip_full_id.toLatin1();
    QByteArray fulldomainflag = fulldomain.toLatin1();
    QByteArray userflag = sip_user.toLatin1();
    QByteArray passwordflag = password.toLatin1();
    QByteArray dominflag = sip_domain.toLatin1();

    pjsua_acc_config_default(&cfg);
    cfg.id = pj_str(fullIDflag.data());
    cfg.reg_uri = pj_str(fulldomainflag.data());
    cfg.cred_count = 1;
    cfg.cred_info[0].realm = pj_str(dominflag.data());
    cfg.cred_info[0].scheme = pj_str("digest");
    cfg.cred_info[0].username = pj_str(userflag.data());
    cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
    cfg.cred_info[0].data = pj_str(passwordflag.data());

    status = pjsua_acc_add(&cfg, PJ_TRUE, &acc_id);
    if (status != PJ_SUCCESS) error_exit("Error adding account", status);
    return status;
}

/* If URL is specified, make call to the URL. */
int simple_pjsua::MakeCall(QString sipID)
{
    QString fullID = "sip:"+sipID;

    QByteArray flag = fullID.toLatin1();
    char *argv = flag.data();           //将QString类型转换为char 型
    status = pjsua_verify_url(argv);
    if (status != PJ_SUCCESS)
        error_exit("Invalid URL in argv", status);
    pj_str_t uri = pj_str(argv);
    status = pjsua_call_make_call(acc_id, &uri, 0, NULL, NULL, NULL);


    if (status != PJ_SUCCESS) error_exit("Error making call", status);
    return status;
}


/* Destroy pjsua */
void simple_pjsua::hangup()
{
    pjsua_call_hangup_all();

}

//-----------------------------------------------------------------------------
void simple_pjsua::hangUp(const int call_id)
{
     pjsua_call_hangup(call_id, 0, 0, 0);
}

/* Callback called by the library upon receiving incoming call */
 void simple_pjsua::on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,pjsip_rx_data *rdata)
{

    qDebug()<<"on_incoming_call";
    qDebug()<<"call_id = "<<call_id;
    my_pjsua_call_id = call_id;
    pjsua_call_info ci;

    PJ_UNUSED_ARG(acc_id);
    PJ_UNUSED_ARG(rdata);

    pjsua_call_get_info(call_id, &ci);

    PJ_LOG(3,(THIS_FILE, "Incoming call from %.*s!!",
             (int)ci.remote_info.slen,
             ci.remote_info.ptr));
    QString IncomingName = QString(QLatin1String(ci.remote_info.ptr));

    pThis->incoming1(IncomingName);


    /* Automatically answer incoming calls with 200/OK */
    //pjsua_call_answer(call_id, 200, NULL, NULL);
}

void simple_pjsua::incoming1(QString Name)
{
    emit Incoming(Name);
}

int simple_pjsua::Answer()
{

    status = pjsua_call_answer(my_pjsua_call_id, 200, NULL, NULL);
    return status ;
}


/* Callback called by the library when call's state has changed */
 void simple_pjsua::on_call_state(pjsua_call_id call_id, pjsip_event *e)
{
     PJ_UNUSED_ARG(e);

     my_pjsua_call_id = call_id;            //wyw***wyw

     pj_status_t status;

     pjsua_call_info ci;
     status = pjsua_call_get_info(call_id, &ci);

     QString state_text = QString::fromLatin1(ci.state_text.ptr,(int)ci.state_text.slen);
     pThis->callstate1(state_text);
     Q_UNUSED(status);
}

void simple_pjsua::callstate1(QString State)
{
    emit CallState(State);
}


/* Callback called by the library when call's media state has changed */
 void simple_pjsua::on_call_media_state(pjsua_call_id call_id)
{
    printf("\n on_call_media_state \n");
    pjsua_call_info ci;
    pjsua_call_get_info(call_id, &ci);
    pThis->media();
    switch (ci.media_status)
    {
    case PJSUA_CALL_MEDIA_NONE:
        PJ_LOG(3,(THIS_FILE, "on_call_media_state: call_id %d: "
                "PJSUA_CALL_MEDIA_NONE: Call currently has no media", call_id));
        break;
    case PJSUA_CALL_MEDIA_ACTIVE:
        PJ_LOG(3,(THIS_FILE, "on_call_media_state: call_id %d: "
                "PJSUA_CALL_MEDIA_ACTIVE: The media is active", call_id));
        // When media is active, connect call to sound device.
        pjsua_conf_connect(ci.conf_slot, 0);
        pjsua_conf_connect(0, ci.conf_slot);
        break;
    case PJSUA_CALL_MEDIA_LOCAL_HOLD:
        PJ_LOG(3,(THIS_FILE, "on_call_media_state: call_id %d: "
                "PJSUA_CALL_MEDIA_LOCAL_HOLD: The media is currently put on hold by local endpoint", call_id));
        break;
    case PJSUA_CALL_MEDIA_REMOTE_HOLD:
        PJ_LOG(3,(THIS_FILE, "on_call_media_state: call_id %d: "
                "PJSUA_CALL_MEDIA_REMOTE_HOLD: The media is currently put on hold by remote endpoint", call_id));
        break;
    case PJSUA_CALL_MEDIA_ERROR:
        PJ_LOG(3,(THIS_FILE, "on_call_media_state: call_id %d: "
                "PJSUA_CALL_MEDIA_ERROR: The media has reported error (e.g. ICE negotiation)", call_id));
        break;
    }
}



/* Display error and exit application */
void simple_pjsua::error_exit(const char *title, pj_status_t status)
{
    pjsua_perror(THIS_FILE, title, status);
    //pjsua_destroy();
    //exit(1);
}



void simple_pjsua::all_device()
{
    pjmedia_aud_dev_info deviceinfo[10];
    unsigned counttt = 10;
    status =  pjsua_enum_aud_devs(deviceinfo,&counttt);
    for(int i=0;i<counttt;i++)
    {
      qDebug()<<"device name :"<<QString::fromLocal8Bit(deviceinfo[i].name)<<"input_count :"<<deviceinfo[i].input_count<<"output_count :"<<deviceinfo[i].output_count;
      //qDebug()<<"output_count:"<<QString::fromLocal8Bit(deviceinfo[i].);
    }
    int capture_dev =0;
    int playback_dev = 0;
    status = pjsua_get_snd_dev(&capture_dev,&playback_dev);
    qDebug()<< "capture_dev = "<<capture_dev;
    qDebug()<< "playback_dev = "<<playback_dev;

    unsigned volume = 0;
        pjsua_snd_get_setting(PJMEDIA_AUD_DEV_CAP_INPUT_VOLUME_SETTING,&volume);
    qDebug()<<"getInputVolume ="<< volume;

    unsigned OutputVolume = 0;
        pjsua_snd_get_setting(PJMEDIA_AUD_DEV_CAP_OUTPUT_VOLUME_SETTING,&OutputVolume);
    qDebug()<<"getOutputVolume ="<<OutputVolume;


}

void simple_pjsua::media()
{
    int capture_dev =0;
    int playback_dev = 0;
    status = pjsua_get_snd_dev(&capture_dev,&playback_dev);
    qDebug()<< "capture_dev = "<<capture_dev;
    qDebug()<< "playback_dev = "<<playback_dev;

    unsigned volume = 0;
        pjsua_snd_get_setting(PJMEDIA_AUD_DEV_CAP_INPUT_VOLUME_SETTING,&volume);
    qDebug()<<"getInputVolume ="<< volume;

    unsigned OutputVolume = 0;
        pjsua_snd_get_setting(PJMEDIA_AUD_DEV_CAP_OUTPUT_VOLUME_SETTING,&OutputVolume);
    qDebug()<<"getOutputVolume ="<<OutputVolume;
}


void simple_pjsua::setSoundSignal(const float soundLevel)
{
    pjsua_conf_port_id slot = 0;
    if (my_pjsua_call_id >= 0) {
        pjsua_call_info ci;
        pjsua_call_get_info(my_pjsua_call_id, &ci);
        slot = ci.conf_slot;

    }

    pjsua_conf_adjust_rx_level(slot, soundLevel);
    //signalSoundLevel(int(soundLevel * 255));
}

//-----------------------------------------------------------------------------
int simple_pjsua::setMicroSignal(const float microLevel)     //2016.6.30 sip初始化问题   未同步
{
    pjsua_conf_port_id slot = -1;
    if (my_pjsua_call_id >= 0) {
        pjsua_call_info ci;
        pjsua_call_get_info(my_pjsua_call_id, &ci);
        slot = ci.conf_slot;
    }
    qDebug()<<"setMicroSignal"<<slot;
    if(slot<0)
    {
        return -1;
    }
    pjsua_conf_adjust_tx_level(slot, microLevel);
    return 0;
    //signalMicroLevel(int(microLevel * 255));
}

void simple_pjsua::getSignalLevels(unsigned int *tx_level, unsigned int *rx_level)
{

    pjsua_conf_port_id slot = 0;
    if (my_pjsua_call_id >= 0) {
        pjsua_call_info ci;
        pjsua_call_get_info(my_pjsua_call_id, &ci);
        slot = ci.conf_slot;
    }
    pjsua_conf_get_signal_level(slot, tx_level, rx_level);
}

//-----------------------------------------------------------------------------
bool simple_pjsua::setSoundDevice(const int input, const int output)
{
    pj_status_t status = pjsua_set_snd_dev(input, output);
    return (status = PJ_SUCCESS);
}

//-----------------------------------------------------------------------------
void simple_pjsua::getSoundDevices(QList<QVariantMap> &device_list)
{
    unsigned dev_count = pjmedia_aud_dev_count();
    pj_status_t status;

    for (unsigned i=0; i<dev_count; ++i)
    {
        QVariantMap device_info;

        pjmedia_aud_dev_info info;

        status = pjmedia_aud_dev_get_info(i, &info);

        if (status != PJ_SUCCESS)
            continue;

        device_info.insert("index", i);
        device_info.insert("name", QString::fromLocal8Bit(info.name));
        device_info.insert("input_count", info.input_count);
        device_info.insert("output_count", info.output_count);

        device_list.append(device_info);
    }
}
void simple_pjsua::getCurrentSoundDevice(int *input_dev, int *output_dev)
{
    pjsua_get_snd_dev(input_dev,output_dev);
}

void simple_pjsua::getDuration(long &total_sec, long &connect_sec)
{
    pj_time_val connect_time;
    pj_time_val total_time;
    if (my_pjsua_call_id >= 0)
    {
        pjsua_call_info ci;
        pjsua_call_get_info(my_pjsua_call_id, &ci);
        connect_time = ci.connect_duration;
        total_time = ci.total_duration;
    }
    connect_sec = connect_time.sec;
    total_sec =  total_time.sec;
}
