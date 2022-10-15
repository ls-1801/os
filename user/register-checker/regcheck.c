#include "../lib/io.h"

void _check_return_jump(void);
void _check_registers(void);
void _check_cpsr(void);
void _check_spsr(void);

/**
 * Register Checker verwendet die nicht blockende printf methode. Für die
 * blockende variante bitte PRINTF mit kprintf definieren.
 */
#define PRINTFN __kprintf

void __register_checker(void) {
  unsigned int cpsr;
  asm("mrs %0, cpsr" : "=r"(cpsr));
  cpsr &= 0x1f;

  PRINTFN("\nBitte sicherstellen, dass waehrend der Tests Interrupts in "
          "ausreichender\n"
          "Menge auftreten und auch durch euch behandelt werden!\n"
          "\n"
          "Die Dauer jedes Tests ist angegeben. Weicht sie stark ab, habt ihr\n"
          "wahrscheinlich ein nicht durch die Tests abgedecktes Problem. "
          "(Zudem haengen\n"
          "CPSR- und General-Purpose-Register-Test sehr voneinander ab: "
          "CPSR-Test braucht\n"
          "Register, Register-Test brauch Flags. Also Registerdump kritisch "
          "betrachten.)\n"
          "\n"
          "Wenn eine Missstand festgestellt wird, wird eine Undef-Exception "
          "ausgelöst.\n"
          "Mittels objdump, Quellen und Registersatz sollte sich das Problem "
          "einkreisen\n"
          "lassen.\n"
          "\n"
          "(Alle Angaben ohne Gewaehr.)\n\n");

  if (cpsr == 0b10010) {
    PRINTFN(
        ">>> Prozessor ist im IRQ-Modus => LR und SPSR verloren nach "
        "Interrupt\n"
        ">>> (Insbesondere geht der Rücksprung aus dem ersten Test schief.)\n"
        "\n"
        ">>> Abbruch!\n");
    return;
  }

  PRINTFN("Pruefe richtige Ruecksprung-Adresse (ca. 5 Sekunden)\n");
  _check_return_jump();

  PRINTFN("Pruefe auf Register-Aenderungen (R0-R14, ca. 10 Sekunden)\n");
  _check_registers();

  if (cpsr == 0b10000) {
    PRINTFN("\n>>> Prozessor ist im User-Modus => Keine Pruefung auf "
            "CPSR-Aenderungen,\n"
            ">>> da dort Modus-Bits geändert werden. (Wie wäre es mit dem "
            "System-Modus?)\n\n");
  } else {
    PRINTFN("Pruefe auf CPSR-Aenderungen (ca. 15 Sekunden)\n");
    _check_cpsr();
  }

  if (cpsr == 0b10000 || cpsr == 0b11111) {
    PRINTFN("\n>>> Prozessor ist im User- oder System-Modus => Keine Pruefung "
            "auf SPSR-Aenderungen,\n"
            ">>> da es dieses Register nicht gibt. (Wie wäre es mit dem "
            "Supervisor-Modus?)\n\n");
  } else {
    PRINTFN("Pruefe auf SPSR-Aenderungen (ca. 15 Sekunden)\n");
    _check_spsr();
  }

  PRINTFN("Fertig! Es _scheint_ so, als wuerde es funktionieren.\n");
}
