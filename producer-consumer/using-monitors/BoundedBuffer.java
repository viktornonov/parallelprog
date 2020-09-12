public interface BoundedBuffer {
    public String take() throws InterruptedException;
    public void put(String payload) throws InterruptedException;
}
