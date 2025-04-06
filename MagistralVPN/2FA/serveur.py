import setproctitle as sp

NOM_PROCESSUS = "MagistralVPN-Serveur"

def setNomProcessus(nom : str):
    sp.setproctitle(nom)

setNomProcessus(NOM_PROCESSUS)
while 1==1:
    pass