import bit
import datetime
import os.path
import random
import subprocess


REPORTS_FOLDER = "reports"


def log(message_):
    message_ = "[{}] {}".format(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"), message_)
    print(message_)
    with open("log.txt", "a") as file:
        file.write(message_ + "\n")


while True:
    while True:
        prefix = random.choice(["K", "L"]) + "".join(random.sample("123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz", 4))
        if "KwDiB" <= prefix < "L5oLm" and not os.path.exists("reports/" + prefix):
            break
    report = ""
    process = subprocess.Popen(["BTC500.exe", prefix], stdout=subprocess.PIPE)
    line = process.stdout.readline()
    while len(line) > 0:
        line = line.decode().strip()
        log(line)
        message_type = line[:4]
        message = line[4:]
        if message_type == "[W] ":
            report += line + "\n"
            if bit.wif_to_key(message).address == "1PfNh5fRcE9JKDmicD2Rh3pexGwce1LqyU":
                log("[i] PRIVATE KEY FOUND!!!")
                input()
        elif message_type == "[E] ":
            report += line + "\n"
        line = process.stdout.readline()
    process.wait()
    if process.returncode != 0:
        line = "[E] Wrong return code: " + str(process.returncode) + "\n"
        log(line)
        report += line
    if not os.path.exists(REPORTS_FOLDER):
        os.makedirs(REPORTS_FOLDER)
    with open(REPORTS_FOLDER + "/" + prefix + ".txt", "w") as file:
        file.write(report)
