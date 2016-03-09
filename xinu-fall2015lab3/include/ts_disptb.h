struct ts_disptb {
  int ts_tqexp;         // new priority: CPU-bound (time quantum expired)
  int ts_slpret;        // new priority: I/O-bound (sleep return)
  int ts_quantum;       // new time slice
};

extern struct ts_disptb tsdtab[]; // declare the struct


