import java.time.Instant;

public class Producer implements Runnable {
    private BoundedBuffer sharedBoundedBuffer;
    private int id;

    public Producer(int id, BoundedBuffer sharedBoundedBuffer) {
        this.id = id;
        this.sharedBoundedBuffer = sharedBoundedBuffer;
    }

    @Override
    public void run() {
        try {
            while (true) {
                String payload = String.valueOf(Instant.now());
                this.sharedBoundedBuffer.put(payload);
                System.out.println("Producer " + id + " puts  " + payload);
                Thread.sleep(1000);
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
