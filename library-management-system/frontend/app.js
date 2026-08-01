const state = {
  search: ''
};

function escapeHtml(value) {
  return String(value)
    .replaceAll('&', '&amp;')
    .replaceAll('<', '&lt;')
    .replaceAll('>', '&gt;')
    .replaceAll('"', '&quot;')
    .replaceAll("'", '&#39;');
}

function loadJsonSource(source) {
  return fetch(source, { cache: 'no-store' }).then((response) => {
    if (!response.ok) {
      throw new Error(source);
    }
    return response.json();
  });
}

async function loadData() {
  const embedded = document.getElementById('library-data');
  if (embedded) {
    return JSON.parse(embedded.textContent);
  }

  for (const source of ['../data/library.json', './data.json']) {
    try {
      return await loadJsonSource(source);
    } catch (error) {
      continue;
    }
  }

  throw new Error('No library data source found');
}

function applyActiveNav(page) {
  document.querySelectorAll('[data-nav]').forEach((link) => {
    const isActive = link.dataset.nav === page;
    link.classList.toggle('is-active', isActive);
  });
}

function summaryBlock(label, value) {
  return `
    <div class="metric-card">
      <p class="metric-label">${escapeHtml(label)}</p>
      <p class="metric-value">${escapeHtml(value)}</p>
    </div>
  `;
}

function bookCard(book) {
  const status = book.isBorrowed ? 'Borrowed' : 'Available';
  const statusClass = book.isBorrowed ? 'status-pill borrowed' : 'status-pill available';

  return `
    <article class="book-card">
      <div class="book-card-top">
        <div>
          <p class="book-card-id">Book #${escapeHtml(book.id)}</p>
          <h4 class="book-card-title">${escapeHtml(book.title)}</h4>
          <p class="book-card-author">${escapeHtml(book.author)}</p>
        </div>
        <span class="${statusClass}">${status}</span>
      </div>
      <button class="secondary-button" type="button">Add to Cart</button>
    </article>
  `;
}

function renderHome(data) {
  const stats = document.getElementById('stats');
  const featured = document.getElementById('featured-books');
  if (stats) {
    stats.innerHTML = data.stats.map((item) => summaryBlock(item.label, item.value)).join('');
  }
  if (featured) {
    const books = (data.books || []).slice(0, 3);
    featured.innerHTML = books.map((book) => bookCard(book)).join('');
  }
}

function renderLogin(data) {
  const cards = document.getElementById('login-cards');
  if (cards) {
    cards.innerHTML = (data.loginCards || []).map((item) => `
      <article class="mini-card">
        <h4 class="mini-card-title">${escapeHtml(item.title)}</h4>
        <p class="mini-card-text">${escapeHtml(item.description)}</p>
      </article>
    `).join('');
  }

  const form = document.getElementById('login-form');
  const status = document.getElementById('login-status');
  if (form && status) {
    form.addEventListener('submit', (event) => {
      event.preventDefault();
      const username = event.target.username.value.trim() || 'Member';
      status.textContent = `Signed in as ${username}.`;
      window.location.href = './account.html';
    });
  }
}

function renderAccount(data) {
  const accountCard = document.getElementById('account-card');
  const activityFeed = document.getElementById('activity-feed');

  if (accountCard) {
    const account = data.account || {};
    accountCard.innerHTML = `
      <div class="info-grid">
        ${summaryBlock('Name', account.name || 'Guest')}
        ${summaryBlock('Member ID', account.memberId || 'LM-0000')}
        ${summaryBlock('Plan', account.plan || 'Standard Reader')}
        ${summaryBlock('Joined', account.joined || 'N/A')}
      </div>
    `;
  }

  if (activityFeed) {
    activityFeed.innerHTML = (data.activity || []).map((entry, index) => `
      <div class="activity-item">
        <span class="activity-index">${index + 1}</span>
        <span class="activity-text">${escapeHtml(entry)}</span>
      </div>
    `).join('');
  }
}

function renderCart(data) {
  const cartItems = document.getElementById('cart-items');
  const cartSummary = document.getElementById('cart-summary');
  if (cartItems) {
    const borrowed = (data.cart || []).length ? data.cart : (data.books || []).filter((book) => book.isBorrowed);
    cartItems.innerHTML = borrowed.map((book) => `
      <article class="cart-item">
        <div class="cart-item-top">
          <div>
            <h4 class="cart-item-title">${escapeHtml(book.title)}</h4>
            <p class="cart-item-author">${escapeHtml(book.author)}</p>
          </div>
          <span class="status-pill borrowed">Borrowed</span>
        </div>
      </article>
    `).join('');
  }

  if (cartSummary) {
    const total = (data.books || []).length;
    const borrowedCount = (data.books || []).filter((book) => book.isBorrowed).length;
    cartSummary.innerHTML = `
      ${summaryBlock('Books in System', String(total))}
      ${summaryBlock('Borrowed Items', String(borrowedCount))}
      ${summaryBlock('Available Books', String(total - borrowedCount))}
    `;
  }
}

function renderCards(data) {
  const searchBox = document.getElementById('search-box');
  const cardsGrid = document.getElementById('all-cards');

  const draw = () => {
    if (!cardsGrid) {
      return;
    }

    const search = state.search.trim().toLowerCase();
    const cards = (data.cards || []).filter((item) => {
      const haystack = `${item.title} ${item.description} ${item.tag}`.toLowerCase();
      return !search || haystack.includes(search);
    });

    cardsGrid.innerHTML = cards.map((item) => `
      <article class="book-card card-shell">
        <p class="book-card-id">${escapeHtml(item.tag)}</p>
        <h4 class="book-card-title">${escapeHtml(item.title)}</h4>
        <p class="book-card-description">${escapeHtml(item.description)}</p>
      </article>
    `).join('');
  };

  if (searchBox) {
    searchBox.addEventListener('input', (event) => {
      state.search = event.target.value;
      draw();
    });
  }

  draw();
}

async function init() {
  const data = await loadData();
  const page = document.body.dataset.page || 'home';

  applyActiveNav(page);

  if (page === 'home') {
    renderHome(data);
  }
  if (page === 'login') {
    renderLogin(data);
  }
  if (page === 'account') {
    renderAccount(data);
  }
  if (page === 'cart') {
    renderCart(data);
  }
  if (page === 'cards') {
    renderCards(data);
  }
}

init().catch((error) => {
  console.error(error);
  document.body.innerHTML = '<div class="p-6 text-white">Unable to load dashboard data.</div>';
});