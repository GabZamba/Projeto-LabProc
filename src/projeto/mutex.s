// Este arquivo foi baseado no exemplo disponível em:
// https://developer.arm.com/documentation/dht0008/a/arm-synchronization-primitives/practical-uses/implementing-a-mutex

.set locked, 0b1
.set unlocked, 0b0

.text

// lock_mutex
// Declare for use from C as extern void lock_mutex(void * mutex)//
.global thread_mutex_lock
thread_mutex_lock:
    mov r1, #locked
load_mutex:
    ldrex   r2, [r0]      // R0 contains the mutex address
    cmp     r2, r1        // Test if mutex is locked
    beq     wait_for_unlock           // If locked - wait for it to be released, from 2
    strexne r2, r1, [r0]  // Not locked, attempt to lock it
    cmpne   r2, #1        // Check if Store-Exclusive failed
    beq     load_mutex           // Failed - retry from 1
    // Lock acquired
    dmb                   // Required before accessing protected resource
    bx      lr

wait_for_unlock:   // Take appropriate action while waiting for mutex to become unlocked
    push    {r0, lr}
    bl      thread_yield
    pop     {r0, lr}
    b       thread_mutex_lock           // Retry from 1

// unlock_mutex
// Declare for use from C as extern void unlock_mutex(void * mutex)//
 .global thread_mutex_unlock
thread_mutex_unlock:
    mov     r1, #unlocked
    dmb                   // Required before releasing protected resource
    str     r1, [r0]      // Unlock mutex
    bx      lr
