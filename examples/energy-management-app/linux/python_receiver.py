import os

PIPE_NAME = "/tmp/chip_evse_led"

def main():
    # Open the named pipe for reading
    print(f"Waiting for data on {PIPE_NAME}...")
    try:
        with open(PIPE_NAME, 'r') as fifo:
            while True:
                # Read data from the pipe
                data = fifo.readline()  # Blocking read; waits for data
                if not data:  # End of stream (sender closed pipe)
                    print("No more data. Exiting.")
                    break

                print(f"Received: {data.strip()}")

    except FileNotFoundError:
        print(f"Pipe {PIPE_NAME} does not exist. Please create it with 'mkfifo'.")
    except KeyboardInterrupt:
        print("\nReceiver interrupted. Exiting.")

if __name__ == "__main__":
    main()
