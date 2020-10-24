import java.util.concurrent.atomic.AtomicInteger;

public class FilterLock {
    private AtomicInteger[] level;
    private AtomicInteger[] victim;
    private int numberOfThreads;

    FilterLock(int numberOfThreads) {
        level = new AtomicInteger[numberOfThreads];
        victim = new AtomicInteger[numberOfThreads];
        this.numberOfThreads = numberOfThreads;

        for (int i = 0; i < numberOfThreads; i++) {
            level[i] = new AtomicInteger();
            victim[i] = new AtomicInteger();
        }
    }

    public void lock() {
        int tid = Integer.parseInt(Thread.currentThread().getName());
        for (int i = 1; i < numberOfThreads; i++) {
            level[tid].set(i);
            victim[i].set(tid);
            for (int k = 0; k < numberOfThreads; k++) {
                while (k != tid && level[k].get() >= i && victim[i].get() == tid) {
                    ;//busy wait
                }
            }
        }
    }

    public void unlock() {
        int tid = Integer.parseInt(Thread.currentThread().getName());
        level[tid].set(0); //start from the lowest level
    }
}
