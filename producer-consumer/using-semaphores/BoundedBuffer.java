import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class BoundedBuffer {
    final Lock bufferLock = new ReentrantLock();
    final Semaphore emptyCount;
    final Semaphore fillCount;

    private String[] fifoBuffer;
    private int capacity;
    private int count = 0;
    private int head = 0;
    private int tail = 0;

    public BoundedBuffer(int capacity) {
        this.capacity = capacity;
        fifoBuffer = new String[capacity];
        fillCount = new Semaphore(0);
        emptyCount = new Semaphore(capacity);
    }

    public String take() throws InterruptedException {
        fillCount.acquire();
        bufferLock.lock();
        String result;
        try {
            result = fifoBuffer[head];
            head++;
            if (head == capacity) {
                head = 0;
            }
        } finally {
            bufferLock.unlock();
        }
        emptyCount.release();
        return result;
    }

    public void put(String payload) throws InterruptedException {
        emptyCount.acquire();
        bufferLock.lock();
        try {
            fifoBuffer[tail] = payload;
            tail++;
            if (tail == capacity) {
               tail = 0;
            }
            count++;
        }
        finally {
            bufferLock.unlock();
        }
        fillCount.release();
    }
}
