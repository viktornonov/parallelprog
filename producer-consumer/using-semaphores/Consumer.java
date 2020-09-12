public class Consumer implements Runnable {
    private BoundedBuffer sharedBoundedBuffer;
    private int id;

    public Consumer(int id, BoundedBuffer sharedBoundedBuffer) {
        this.id = id;
        this.sharedBoundedBuffer = sharedBoundedBuffer;
    }

    @Override
    public void run() {
        try {
            while (true) {
                String payload = this.sharedBoundedBuffer.take();
                System.out.println("Consumer " + id + " takes " + payload);
                Thread.sleep(1000);
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
