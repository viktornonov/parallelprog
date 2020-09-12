public class BoundedBufferWithSync implements BoundedBuffer {
    private String[] fifoBuffer;
    private int capacity;
    private int count = 0;
    private int head = 0;
    private int tail = 0;

    public BoundedBufferWithSync(int capacity) {
        this.capacity = capacity;
        fifoBuffer = new String[capacity];
    }

    public synchronized String take() throws InterruptedException {
        while (count == 0) {
            wait();
        }
        String result = fifoBuffer[head];
        head++;
        if (head == capacity) {
            head = 0;
        }
        count--;
        notifyAll();
        return result;
    }

    public synchronized void put(String payload) throws InterruptedException {
        while (count == capacity) {
            wait();
        }
        fifoBuffer[tail] = payload;
        tail++;
        if (tail == capacity) {
           tail = 0;
        }
        count++;
        notifyAll();
    }
}
