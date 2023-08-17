import configparser

config = configparser.ConfigParser()
config.read('platformio.ini')

for section in config.sections():
    if section.startswith('env:'):
        print(section.split(':')[1])
