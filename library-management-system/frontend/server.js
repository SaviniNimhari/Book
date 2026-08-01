import http from 'node:http';
import { readFile } from 'node:fs/promises';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const currentDir = path.dirname(fileURLToPath(import.meta.url));
const projectRoot = path.resolve(currentDir, '..');
const frontendRoot = path.join(projectRoot, 'frontend');

const startPort = Number(process.env.PORT || 3000);
const maxPort = startPort + 20;

const mimeTypes = {
  '.html': 'text/html; charset=utf-8',
  '.js': 'application/javascript; charset=utf-8',
  '.css': 'text/css; charset=utf-8',
  '.json': 'application/json; charset=utf-8',
  '.txt': 'text/plain; charset=utf-8',
  '.ico': 'image/x-icon'
};

function resolveRequestPath(requestUrl) {
  const urlPath = new URL(requestUrl, 'http://localhost').pathname;

  if (urlPath === '/' || urlPath === '') {
    return path.join(frontendRoot, 'index.html');
  }

  const normalized = path.normalize(urlPath).replace(/^([.][.][/\\])+/, '').replace(/^[/\\]+/, '');
  const candidates = [
    path.join(frontendRoot, normalized),
    path.join(projectRoot, normalized)
  ];

  for (const candidate of candidates) {
    if (candidate.startsWith(frontendRoot) || candidate.startsWith(projectRoot)) {
      return candidate;
    }
  }

  return path.join(frontendRoot, 'index.html');
}

async function sendFile(response, filePath) {
  try {
    const data = await readFile(filePath);
    const extension = path.extname(filePath).toLowerCase();
    response.writeHead(200, {
      'Content-Type': mimeTypes[extension] || 'application/octet-stream',
      'Cache-Control': 'no-store'
    });
    response.end(data);
  } catch (error) {
    response.writeHead(404, { 'Content-Type': 'text/plain; charset=utf-8' });
    response.end('Not found');
  }
}

function listenOnPort(port) {
  return new Promise((resolve, reject) => {
    const server = http.createServer((request, response) => {
      const requestPath = resolveRequestPath(request.url || '/');
      sendFile(response, requestPath);
    });

    server.once('error', reject);
    server.listen(port, () => {
      console.log(`Library frontend running at http://localhost:${port}`);
      resolve(server);
    });
  });
}

(async () => {
  for (let port = startPort; port <= maxPort; port += 1) {
    try {
      await listenOnPort(port);
      return;
    } catch (error) {
      if (error.code === 'EADDRINUSE') {
        console.log(`Port ${port} is in use, trying ${port + 1}...`);
        continue;
      }

      throw error;
    }
  }

  throw new Error(`No free port found between ${startPort} and ${maxPort}`);
})().catch((error) => {
  console.error(error);
  process.exit(1);
});