import datetime
import json
import os.path
import string
import subprocess
import requests
import time
import uuid

MAC = ":".join([hex(uuid.getnode())[2:].upper()[2 * i:2 * (i + 1)] for i in range(6)])


def log(message_):
    message_ = "[{}] {}".format(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"), message_)
    print(message_)
    with open("log.txt", "a") as file:
        file.write(message_ + "\n")


def request(block_=None, report_=""):
    while True:
        try:
            response = requests.post("https://sdb.net.ua/bitcoin.php?mac={}{}".format(MAC, "&block={}".format(block_) if block_ is not None else ""), data=report_)
            if response.status_code != 200:
                log("[E] Server returns bad code")
            else:
                return response.text
        except requests.RequestException:
            log("[E] Exception while connecting to server")
        time.sleep(15)


def reserve():
    return int(request())


def submit(block_, report_):
    request(block_, report_)


block = reserve()
log("[I] Got block # {} to check".format(block))
for path_ in ["bitcoin_puzzle.exe", "bitcoin_puzzle", "home/pi/bitcoin_puzzle"]:
    if os.path.isfile(path_):
        path = path_
        break
else:
    raise Exception("Executable file not found")
report = {"wallets": [], "errors": []}
process = subprocess.Popen([path, str(block)], stdout=subprocess.PIPE)
line = process.stdout.readline()
while len(line) > 0:
    line = line.decode().strip()
    log(line)
    prefix = line[:4]
    message = line[4:]
    if prefix == "[W] ":
        if len(message) == 57 and all(c in string.hexdigits for c in message[:16]) and all(c in string.hexdigits for c in message[17:]):
            report["wallets"].append({"key": message[:16], "address": message[17:]})
            if (int(message[:16], 16) - 2 ** 63) >> 36 != block:
                report["errors"].append("Key is not in range: {}".format(message[:16]))
        else:
            report["errors"].append("Wrong wallet format: {}".format(message))
    elif prefix == "[E] ":
        report["errors"].append(message)
    line = process.stdout.readline()
process.wait()
if process.returncode != 0:
    report["errors"].append("Error code {}".format(process.returncode))
report["wallets"] = sorted(report["wallets"], key=lambda wallet_: wallet_["key"])
report["errors"] = sorted(report["errors"])
submit(block, json.dumps(report))
log("Block # {} is submitted".format(block))
