public class Printer implements Runnable {
    private FilterLock filterLock;

    Printer(FilterLock filterLock) {
        this.filterLock = filterLock;
    }

    public void run() {
        while(true) {
            filterLock.lock();
            String tid = Thread.currentThread().getName();
            System.out.println("Thread " + tid + " is in critical section");
            System.out.println("Thread " + tid + " is getting out of critical section");
            filterLock.unlock();
        }
    }
}
