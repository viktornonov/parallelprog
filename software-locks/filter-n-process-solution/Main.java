public class Main {
    public static void main(String[] args) {
        int numberOfThreads = 7;
        FilterLock filterLock = new FilterLock(numberOfThreads);
        Printer printer = new Printer(filterLock);

        for (int i = 0; i < numberOfThreads; i++) {
            Thread t = new Thread(printer, String.valueOf(i));
            t.start();
        }
    }
}
