import setproctitle as sp
import time

NOM_PROCESSUS = "MagistralVPN-Serveur"

def setNomProcessus(nom : str):
    sp.setproctitle(nom)

def genererCode() -> int:
    return int(str(time.time())[-7:])

def sendCode(code : int):
    # Logique d'implémentation :
    # Envoi du code par Télégram, Whatsapp, ou autre chose.
    pass

def getCode(code_initial : int, code_recu : int) -> bool:
    if int(code_initial) == int(code_recu):
        return True
    else:
        return False

setNomProcessus(NOM_PROCESSUS)
code_genere = genererCode()

while True:
    pass