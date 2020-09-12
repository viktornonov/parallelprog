class Program {
    final static int PRODUCERS_COUNT = 5;
    final static int CONSUMERS_COUNT = 10;

    public static void main(String args[]) {
        System.out.println("Starting");
        //BoundedBuffer buffer = new BoundedBufferWithConditions(5);
        BoundedBuffer buffer = new BoundedBufferWithSync(5);
        for (int i = 0; i < PRODUCERS_COUNT; i++) {
            Producer producer = new Producer(i, buffer);
            Thread t = new Thread(producer);
            t.start();
        }

        for (int i = 0; i < CONSUMERS_COUNT; i++) {
            Consumer consumer = new Consumer(i, buffer);
            Thread t = new Thread(consumer);
            t.start();
        }
    }
}
