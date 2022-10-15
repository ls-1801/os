# Aufgabe 3
1. [x] Threadverwaltung:
    - In system/thread.c
    - Unterstützt 32 Threads
        - static heap mit platz für 32 Threads.
    - Welche Variante?
        - Jeder Thread bekommt seinen eigenen Stack. (100KB stack) 
    - Neuen Thread starten
        - in scheduler.c
        - start_thread(fnPtr, void ptr, usize len) methode.
        - R0 - R12 bekommen Debug Wert.
        - SP wird auf den Anfang des Thread Stack gelegt.
            - bzw. erst werden die Argumente auf den Stack Kopiert, dann der Stack auf 4 Byte ausgerichtet.
        - CPSR TBD. (Alle StatusBits aus. User Mode. IRQ und FIQ nicht maskiert. Endieness??)
        - LR auf KILLER FUNKTION. (Syscall für kill_thread)
        - PC auf Functions Ptr.
2. [x] Kontextwechsel
    - Thread Context speichern: 
        - R0 - R12 werden zu beginn des IRQ auf den Stackgelegt.
        - SP und LR müssen durch wechsel in Sys Mode erhalten werden.
        - PC ist das LR - 4.
        - CPSR liegt im SPSR des IRQ.
    - Umsetzung
        - SPSR und Pointer auf Stackadresse mit den Registern werden in den globals __current_thread_spsr und __current_thread_registers gespeichert.
        - Bei IRQ werden alle nötigen Register Gesammelt, um den TCB upzudaten.
        - Zusätzlich müssen für den CS auch der Pointer auf die Register im IRQ Stack mitgeben werden, da die ja evtl. ersetzt werden 
        müssen.
        - Stack Pointer und LR müssen im IRQ durch den SYS mode für den neuen Thread ersetzt werden.
        - der CPSR des Threads wird in den SPSR des IRQ geschrieben.

3. [x] Scheduler
    - Round Robin
        - Linked List FIFO implementation.
    - Idle Thread
        - Wann initialisieren?
            - start funktion in kernel.c wird zum idle thread.
        - Idle thread mittels WFI im stromsparmodus
    - Konfigurierbare Zeitscheiben nur im Quellcode
        - scheduler.c in start_schedule
4. [x] Funktion für Thread erstellung
    - in scheduler.c + thread.c
    - Beliebige Argumente, bei start in Thread Stack kopieren
    - Thread läuft im User Modus.
5. [x] Thread Beenden. Idle Thread.
    - Thread wird durch einen Syscall beendet.
    - LR Register wird zu beginn des Threads auf eine Killer Funktion gesetzt, die den Thread beendet.
    - Idle Thread wird nie beendet.
6. [x] Exception Handling.
    - Händler zieht LR offset ab und legt user register auf den stack.
    - Spsr und ptr für die register werden in gobale variabeln kopiert.
    - Die Exception Handler müssen dem IRQ angepasst werden.
    - Abort und Undefined brauchen einen kill_thread methode. SVC auch.
        - Exception im Thread: Register Dump, Thread beenden
        - Exception in System: Register Dump, Halt
    - S, A, U für Exceptions im Uart Interrupt Handler.
7. [x] Thread erzeugen innerhalb der Uart Interrupt behandlung.
    - Einstiegspunkt user_thread(), empfangendes zeichen als Argument.
8. [x] Thread soll das empfangende Zeichen wiederholen, mit pause
    - eingabe s, a, u, c sollen im thread exceptions auslösen.
9. [x] Timer interrupt soll ein Ausrufe Zeichen ausgeben, Kontextwechsel 
zeilenumbruch.