import configparser

config = configparser.ConfigParser()
config.read('platformio.ini')

boards = [section.split(":")[1].strip() for section in config.sections() if section.startswith("env:")]

print(" ".join(boards))