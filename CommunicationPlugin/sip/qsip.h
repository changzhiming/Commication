#ifndef SIMPLE_PJSUA_H
#define SIMPLE_PJSUA_H

#include <QObject>
#include <pjsua-lib/pjsua.h>

#define MAX_MEDIA_CNT	2	     /* Media count, set to 1 for audio*/
class simple_pjsua : public QObject
{
    Q_OBJECT
public:
    explicit simple_pjsua(QObject *parent = 0);
    ~simple_pjsua();

    int SipInit();                                                         //初始化SIP电话，返回值若为0，则成功
    int RegisterSIP(QString sip_user,QString password,QString sip_domain); //登录(输入参数为：用户名、密码、服务器地址)，返回值若为0，则成功
                                                                           //界面需要对输入参数进行基本的格式判断
    int MakeCall(QString sipID);                                           //拨打电话(输入参数为：对方sip地址)
    int Answer();                                                          //接听电话
    void hangup();                                                         //挂断电话
    void hangUp(const int call_id);
    void setSoundSignal(const float soundLevel);                           //设置输出声音音量大小
    int setMicroSignal(const float microLevel);                           //设置输入麦克风音量大小
    void getSignalLevels(unsigned int *tx_level, unsigned int *rx_level);
    bool setSoundDevice(const int input, const int output);
    //void getCurrentSoundDevice(int *input_dev, int *output_dev);
    void getCurrentSoundDevice(int *input_dev, int *output_dev) ;

    void getSoundDevices(QList<QVariantMap> &device_list);
    void getDuration(long &total_sec, long &connect_sec);


signals:
    void Incoming(QString incoming_name);                                   //来电信号，参数为来电的sip地址
    void CallState(QString state);

public slots:
public:
    pj_status_t status=0;

private:
    pjsua_acc_id acc_id;

    static simple_pjsua *pThis; //指向该类的指针
    static  int my_pjsua_call_id;

    int Create();
    int Init();
    int AddUDP();
    int StartPjsua();
    void incoming1(QString Name);
    void callstate1(QString State);
    static void error_exit(const char *title, pj_status_t status);
    static void on_call_media_state(pjsua_call_id call_id);
    static void on_call_state(pjsua_call_id call_id, pjsip_event *e);
    static void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,pjsip_rx_data *rdata);
    void all_device();
    void media();
};

#endif // SIMPLE_PJSUA_H
