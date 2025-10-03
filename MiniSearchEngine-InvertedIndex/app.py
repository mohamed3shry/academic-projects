from flask import Flask, render_template, request
import os

app = Flask(__name__)

# --- Step 0: Load document links ---
def load_doc_links(file_path):
    doc_links = {}
    with open(file_path, 'r', encoding='utf-8') as f:
        for line in f:
            if ':' not in line:
                continue
            idx, link = line.strip().split(':', 1)
            try:
                doc_links[int(idx.strip())] = link.strip()
            except:
                continue
    return doc_links

# --- Step 1: Load the inverted index from file ---
def load_inverted_index(file_path):
    inverted_index = {}
    with open(file_path, 'r', encoding='utf-8') as f:
        for line in f:
            if not line.strip():
                continue
            parts = line.strip().split('\t')
            if len(parts) != 2:
                continue
            word, occurrences = parts
            doc_dict = {}
            for entry in occurrences.split(';'):
                if '→' in entry:
                    count, doc_id = entry.split('→')
                    try:
                        count = int(count)
                        doc_id = int(doc_id)
                        if doc_id in doc_dict:
                            doc_dict[doc_id] += count
                        else:
                            doc_dict[doc_id] = count
                    except:
                        continue
            inverted_index[word] = doc_dict
    return inverted_index

# --- Load index and links at startup ---
INDEX_FILE = 'indexer.txt'
LINKS_FILE = 'urls.txt'

inverted_index = load_inverted_index(INDEX_FILE)
doc_links = load_doc_links(LINKS_FILE)

# --- Step 2: Web interface ---
@app.route('/', methods=['GET', 'POST'])
def home():
    query = request.form.get('query', '').strip()
    results = []
    if query:
        word = query.upper()  # تعامل مع الكلمات كأنها كابيتال
        if word in inverted_index:
            data = inverted_index[word]
            results = sorted([
                (doc_links.get(doc_id, f"وثيقة {doc_id}"), count)
                for doc_id, count in data.items()
            ], key=lambda x: -x[1])  # ترتيب بالتكرار
    return render_template('index.html', query=query, results=results)

if __name__ == '__main__':
    app.run(debug=True)  # شغل على localhost
