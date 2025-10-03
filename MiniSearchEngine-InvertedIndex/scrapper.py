import requests
from bs4 import BeautifulSoup
import os

def fetch_and_save_links(doc_links_file, output_folder):
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    with open(doc_links_file, 'r', encoding='utf-8') as f:
        for line in f:
            if ':' not in line:
                continue
            idx, url = line.strip().split(':', 1)
            idx = idx.strip()
            url = url.strip()

            try:
                response = requests.get(url, timeout=10)
                if response.status_code == 200:
                    soup = BeautifulSoup(response.text, 'html.parser')

                    for tag in soup(['script', 'style', 'noscript']):
                        tag.decompose()

                    text = soup.get_text(separator=' ', strip=True)

                    with open(f"{output_folder}/{idx}.txt", 'w', encoding='utf-8') as out_file:
                        out_file.write(text)
                    
                    print(f"[✓] Saved: {idx}.txt")
                else:
                    print(f"[✗] Failed to fetch: {url} - Status: {response.status_code}")
            except Exception as e:
                print(f"[✗] Error with {url}: {e}")

if __name__ == '__main__':
    fetch_and_save_links('urls.txt', 'documents')
