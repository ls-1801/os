## Blatt 4
- [] Aufrufkonvention für SWI.
    - argumente in r0, r1, r2, r3 bei mehr struct.
    - return durch r0.
- [] SWI:
    - write
        - char* + length
    - read
        - char* + length
    - thread beenden
    - thread delay
        - int inMillis
- [] Implementierung auf kernel seite.
- [] Trennung von Syscalls und implementierung im kernel.
- [] Threads blockieren
    - blocking queue.
    - Wie weiß ich, dass ein Thread nicht mehr blockiert ist? Callback nach syscall?