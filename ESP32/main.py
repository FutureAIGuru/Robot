import ujson
import utime

from ota_update.main.ota_updater import OTAUpdater

@staticmethod
def _otaUpdate():
    from .ota_updater import OTAUpdater
    otaUpdater = OTAUpdater('https://github.com/FutureAIGuru/Robot', 
                            github_src_dir='ESP32', 
                            main_dir='app', 
                            secrets_file="config.json")
    otaUpdater.install_update_if_available()
    del(otaUpdater)

def download_and_install_update_if_available(config_data):
    if 'wifi' in config_data:
        o = OTAUpdater('https://github.com/FutureAIGuru/Robot')
        o.download_and_install_update_if_available(config_data['wifi']['ssid'], config_data['wifi']['password'])
    else:
        print('No WIFI configured, skipping updates check')


def start(config_data):
    global s
    utime.sleep_ms(10000)
    from main.RobotBrain import RobotBrain
    print("experimental update")
    s = RobotBrain(config_data)


def boot_RobotBrain():
    f = open('config.json')
    config_data = ujson.load(f)

    download_and_install_update_if_available(config_data)
    start(config_data)


s = None
boot_RobotBrain()