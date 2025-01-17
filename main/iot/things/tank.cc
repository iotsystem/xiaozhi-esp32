#include "iot/thing.h"
#include "board.h"
#include "audio_codec.h"

#include <driver/gpio.h>
#include <esp_log.h>
#include "tracked_chassis_control.h"

#define TAG "Tank"

namespace iot {

// 这里仅定义 Tank 的属性和方法，不包含具体的实现
class Tank : public Thing {
private:
    bool power_ = false;
    int direction_ = 0;
    int speed_ = 0;
    

public:
    Tank() : Thing("Tank", "坦克控制"), power_(false) {
        tracked_chassis_control_start();
        // 定义设备的属性
        properties_.AddBooleanProperty("power", "坦克灯光是否打开", [this]() -> bool {
            return power_;
        });
        properties_.AddNumberProperty("speed", "坦克运行速度", [this]() -> int {
            return speed_;
        });

        // 定义设备可以被远程执行的指令
        methods_.AddMethod("TurnOn", "打开坦克灯光", ParameterList(), [this](const ParameterList& parameters) {
            power_ = true;
            tracked_chassis_rgb_light_control(2);
        });

        methods_.AddMethod("TurnOff", "关闭坦克灯光", ParameterList(), [this](const ParameterList& parameters) {
            power_ = false;
            tracked_chassis_rgb_light_control(8);            
        });
        methods_.AddMethod("GoForward", "向前", ParameterList(), [this](const ParameterList& parameters) {
            direction_ = 1;
            tracked_chassis_motion_control("x0.0y1.0");
            vTaskDelay(500 / portTICK_PERIOD_MS);
            tracked_chassis_motion_control("x0.0y0.0");     
        });
        methods_.AddMethod("GoBack", "向后", ParameterList(), [this](const ParameterList& parameters) {
            direction_ = 2;
            tracked_chassis_motion_control("x0.0y-1.0");
            vTaskDelay(500 / portTICK_PERIOD_MS);
            tracked_chassis_motion_control("x0.0y0.0");
        });

        methods_.AddMethod("GoLeft", "向左", ParameterList(), [this](const ParameterList& parameters) {
            direction_ = 3;
            tracked_chassis_motion_control("x-1.0y0.0");
            vTaskDelay(600 / portTICK_PERIOD_MS);
            tracked_chassis_motion_control("x0.0y0.0");
        });
        methods_.AddMethod("GoRight", "向右", ParameterList(), [this](const ParameterList& parameters) {
            direction_ = 4;
            tracked_chassis_motion_control("x1.0y0.0");
            vTaskDelay(600 / portTICK_PERIOD_MS);
            tracked_chassis_motion_control("x0.0y0.0");
        });
        methods_.AddMethod("GoDance", "跳个舞", ParameterList(), [this](const ParameterList& parameters) {
            tracked_chassis_set_dance_mode(1);
        });

        methods_.AddMethod("SetSpeed", "设置坦克运行速度", ParameterList({
            Parameter("speed", "1到100之间的整数", kValueTypeNumber, true)
        }), [this](const ParameterList& parameters) {
            int speed = static_cast<uint8_t>(parameters["speed"].number());
            ESP_LOGI(TAG,"速度->%d",speed);
        });

        methods_.AddMethod("LightShow", "灯光秀", ParameterList(), [this](const ParameterList& parameters) {
                tracked_chassis_rgb_light_control(7);
                vTaskDelay(10000 / portTICK_PERIOD_MS);
                tracked_chassis_rgb_light_control(8);
        }); 
        methods_.AddMethod("SetBrightness", "设置坦克灯光亮度", ParameterList({
            Parameter("brightness", "1到100之间的整数", kValueTypeNumber, true)
        }), [this](const ParameterList& parameters) {
            int brightness = static_cast<uint8_t>(parameters["brightness"].number());
            ESP_LOGI(TAG,"亮度->%d",brightness);  
        });
    }
};

} // namespace iot

DECLARE_THING(Tank);
