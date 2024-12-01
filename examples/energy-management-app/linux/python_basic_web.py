from http.server import BaseHTTPRequestHandler, HTTPServer
import urllib.parse

class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        # Parse the query string
        parsed_path = urllib.parse.urlparse(self.path)
        query_params = urllib.parse.parse_qs(parsed_path.query)

        # Get the value of the `PL` variable
        pl_value = query_params.get("PL", [""])[0]  # Default to empty string if PL is not present

        # Send HTTP response
        self.send_response(200)  # HTTP 200 OK
        self.send_header("Content-Type", "text/plain; charset=utf-8")
        self.end_headers()

        # Response message
        response = f"Received PL value: {pl_value}"
        self.wfile.write(response.encode("utf-8"))  # Write response as bytes

# Configure and run the server
def run(server_class=HTTPServer, handler_class=SimpleHTTPRequestHandler, port=8080):
    server_address = ("", port)
    httpd = server_class(server_address, handler_class)
    print(f"Starting HTTP server on port {port}...")
    httpd.serve_forever()

if __name__ == "__main__":
    run()
