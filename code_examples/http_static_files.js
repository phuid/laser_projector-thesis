const http = require('http');
const fs = require('fs');
const path = require('path');

const server = http.createServer((req, res) => {
  // Get the file path from the request URL
  const filePath = path.join(__dirname, req.url);

  // Check if the file exists
  fs.access(filePath, fs.constants.F_OK, (err) => {
    if (err) {
      // File not found
      res.statusCode = 404;
      res.end('File not found');
    } else {
      // Read the file and send it as the response
      fs.readFile(filePath, (err, data) => {
        if (err) {
          // Error reading the file
          res.statusCode = 500;
          res.end('Internal server error');
        } else {
          // Set the appropriate content type and send the file data
          const ext = path.extname(filePath);
          let contentType = 'text/plain';
          if (ext === '.html') {
            contentType = 'text/html';
          } else if (ext === '.css') {
            contentType = 'text/css';
          } else if (ext === '.js') {
            contentType = 'text/javascript';
          }

          res.setHeader('Content-Type', contentType);
          res.end(data);
        }
      });
    }
  });
});

const port = 3000;
server.listen(port, () => {
  console.log(`Server running on port ${port}`);
});
