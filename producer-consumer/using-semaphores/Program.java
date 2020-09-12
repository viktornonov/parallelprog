class Program {
    final static int PRODUCER_COUNT = 5;
    final static int CONSUMER_COUNT = 5;

    public static void main(String args[]) {
        System.out.println("Starting");
        BoundedBuffer buffer = new BoundedBuffer(5);
        for (int i = 0; i < PRODUCER_COUNT; i++) {
            Producer producer = new Producer(i, buffer);
            Thread t = new Thread(producer);
            t.start();
        }

        for (int i = 0; i < CONSUMER_COUNT; i++) {
            Consumer consumer = new Consumer(i, buffer);
            Thread t = new Thread(consumer);
            t.start();
        }
    }
}
