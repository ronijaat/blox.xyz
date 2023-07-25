import time
import threading

class TokenBucket:
    def __init__(self, capacity, refill_rate):
        self.capacity = capacity
        self.refill_rate = refill_rate
        self.tokens = capacity
        self.last_refill_time = time.time()
        self.lock = threading.Lock()

    def take_tokens(self, num_tokens):
        with self.lock:
            now = time.time()
            time_elapsed = now - self.last_refill_time
            self.tokens = min(self.capacity, self.tokens + time_elapsed * self.refill_rate)
            self.last_refill_time = now

            if self.tokens >= num_tokens:
                self.tokens -= num_tokens
                return True
            else:
                return False

# Example usage:
api_rate_limit = 15  # Max allowed calls per minute
token_refill_rate = api_rate_limit / 60  # Refill rate per second

# Create a token bucket with the desired rate limit and refill rate
token_bucket = TokenBucket(api_rate_limit, token_refill_rate)

def call_me(string_input):
    if token_bucket.take_tokens(1):
        # Make the API call here
        print("API call successful.")
    else:
        # Handle rate-limiting, maybe wait or return an error response
        print("API rate limit exceeded. Please wait and try again later.")

# Test the API calls with bursts
def make_api_calls():
    for _ in range(30):
        call_me("sample input")
        time.sleep(2)  # Simulate time between API calls

threads = [threading.Thread(target=make_api_calls) for _ in range(3)]

# Start the threads to simulate multiple users making API calls concurrently
for thread in threads:
    thread.start()

# Wait for all threads to complete
for thread in threads:
    thread.join()
