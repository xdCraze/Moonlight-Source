import os, time
try:
    import pyinjector
    import psutil
    from colorama import Fore
    import colorama
    colorama.init()
except ImportError:
    print("[i] Some modules were not installed... Installing...")
    print("[i] Installing colorama...")
    os.system("pip install colorama")
    from colorama import Fore
    print(f"{Fore.LIGHTCYAN_EX}[{Fore.RESET}i{Fore.LIGHTCYAN_EX}]{Fore.RESET} Installing {Fore.MAGENTA}pyinjector{Fore.RESET}...")
    os.system("pip install pyinjector")
    import pyinjector
    print(f"{Fore.LIGHTCYAN_EX}[{Fore.RESET}i{Fore.LIGHTCYAN_EX}]{Fore.RESET} Installing {Fore.MAGENTA}psutil{Fore.RESET}...")
    os.system("pip install psutil")
    import psutil

INJECTDELAY: int = 1 # Second
PROCESS: str = "Pixel Gun 3D.exe"
FOLDERPATH: str = os.getcwd()

def log(message: str):
    print(f"{Fore.LIGHTCYAN_EX}[INFO{Fore.LIGHTCYAN_EX}]{Fore.RESET} {message}")
def logerror(message: str):
    print(f"{Fore.LIGHTRED_EX}[ERROR{Fore.LIGHTRED_EX}]{Fore.RESET} {message}")
def logwarning(message: str):
    print(f"{Fore.LIGHTYELLOW_EX}[WARNING{Fore.LIGHTYELLOW_EX}]{Fore.RESET} {message}")

def get_pid() -> set:
    pid = 0
    for proc in psutil.process_iter():
        if proc.name() == PROCESS:
            try:
                proc.memory_full_info().private
                pid = proc.pid
                return pid
            except:
                pass
    return pid

log("Moonlight Injector (Made By @YeetDisDude)\nHe gave me perms to use it lol\n\n")

def inject(pid: int) -> bool:
    try:
        pyinjector.inject(pid, "Moonlight.dll")
        return True
    except Exception as e:
        logerror(f"Injection failed, Error: {e}")
        log("Possible fixes:")
        if "Could not find" in str(e):
            print(f" - Extract the ZIP{Fore.LIGHTRED_EX}{' (temp folder detected!)' if 'Temp' in FOLDERPATH else ''}{Fore.RESET}\n - Ensure that the DLL is in the same folder as the injector\n")
        elif "virus" in str(e):
            print(" - Disable Windows Defender")
        else:
            print(" - None")
        return False

injected: bool = False
printednotfound: bool = False
if __name__ == "__main__":
    os.system("title Moonlight Injector (Made by @YeetDisDude)")
    log("Open Pixel Gun 3D to inject")
    while True:
        pid = get_pid()
        if pid > 0:
            if injected == False:
                log(f"Found Pixel Gun 3D, Injecting in {INJECTDELAY}s...")
                time.sleep(INJECTDELAY)
                r = inject(pid)
                injected = True
                printednotfound = False
                if r:
                    log("Injected!\n")
        else:
            if printednotfound == False:
                logwarning("Pixel Gun 3D not found! Waiting for the game to be opened...\n")
                printednotfound = True
            injected = False
        time.sleep(2)