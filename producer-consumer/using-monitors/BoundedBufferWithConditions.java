import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class BoundedBufferWithConditions implements BoundedBuffer {
    final Lock bufferLock = new ReentrantLock();
    final Condition bufferNotFull = bufferLock.newCondition();
    final Condition bufferNotEmpty = bufferLock.newCondition();

    private String[] fifoBuffer;
    private int capacity;
    private int count = 0;
    private int head = 0;
    private int tail = 0;

    public BoundedBufferWithConditions(int capacity) {
        this.capacity = capacity;
        fifoBuffer = new String[capacity];
    }

    public String take() throws InterruptedException {
        bufferLock.lock();
        try {
            while (count == 0) {
                bufferNotEmpty.await();
            }
            String result = fifoBuffer[head];
            head++;
            if (head == capacity) {
                head = 0;
            }
            count--;
            bufferNotFull.signalAll();
            return result;
        } finally {
            bufferLock.unlock();
        }
    }

    public void put(String payload) throws InterruptedException {
        bufferLock.lock();
        try {
            while (count == capacity) {
                bufferNotFull.await();
            }
            fifoBuffer[tail] = payload;
            tail++;
            if (tail == capacity) {
               tail = 0;
            }
            count++;
            bufferNotEmpty.signalAll();
        } finally {
            bufferLock.unlock();
        }
    }
}
