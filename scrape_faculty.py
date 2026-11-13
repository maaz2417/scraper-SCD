"""Scrape public faculty details and profile photos from FAST-NUCES Peshawar."""

from pathlib import Path
import csv
import re
from urllib.parse import urljoin, urlparse

import requests
from bs4 import BeautifulSoup


FACULTY_URL = "https://pwr.nu.edu.pk/cs-faculty/"
UNIVERSITY = "FAST - National University of Computer and Emerging Sciences (Peshawar Campus)"
OUTPUT_DIR = Path(__file__).resolve().parent
IMAGE_DIR = OUTPUT_DIR / "faculty_images"
CSV_PATH = OUTPUT_DIR / "faculty_data.csv"
HEADERS = {"Name": "", "Designation": "", "Department": "Computer Science", "Email": "", "Contact": ""}


def main():
    response = requests.get(FACULTY_URL, headers={"User-Agent": "Mozilla/5.0 (educational faculty directory scraper)"}, timeout=30)
    response.raise_for_status()
    soup = BeautifulSoup(response.text, "html.parser")
    cards = soup.select(".faculty-member")
    if not cards:
        raise RuntimeError("No .faculty-member cards found; the university may have changed its page layout.")

    IMAGE_DIR.mkdir(parents=True, exist_ok=True)
    rows = []
    for card in cards:
        details = card.select_one(".faculty-details") or card
        name_node = details.select_one("h2 a, h2")
        if not name_node:
            continue
        name = name_node.get_text(" ", strip=True)
        paragraphs = [p.get_text(" ", strip=True) for p in details.select("p") if p.get_text(" ", strip=True)]
        email = next((p for p in paragraphs if re.search(r"\b[\w.+-]+@[\w.-]+\.[A-Za-z]{2,}\b", p)), "")
        phone = next((p for p in paragraphs if re.search(r"(?:\+?\d[\d ()-]{6,}\d)", p)), "")
        designation = next((p for p in paragraphs if p != email and p != phone and "Approved PhD Supervisor" not in p), "")
        rows.append({"Name": name, "Designation": designation, "Department": "Computer Science", "Email": email, "Contact": phone})

        img = card.select_one("img")
        if img and (img.get("src") or img.get("data-src")):
            # The site's image paths are rooted at the domain, even though
            # they are written as relative paths in the page HTML.
            image_url = urljoin(FACULTY_URL, "/" + (img.get("src") or img.get("data-src")).lstrip("/"))
            suffix = Path(urlparse(image_url).path).suffix or ".jpg"
            safe_name = re.sub(r"[^A-Za-z0-9_-]+", "_", name).strip("_")
            image_path = IMAGE_DIR / f"{safe_name}{suffix}"
            try:
                image_response = requests.get(image_url, headers={"User-Agent": "Mozilla/5.0"}, timeout=30)
                image_response.raise_for_status()
                image_path.write_bytes(image_response.content)
            except requests.RequestException as exc:
                print(f"Could not download photo for {name}: {exc}")

    with CSV_PATH.open("w", newline="", encoding="utf-8-sig") as f:
        writer = csv.DictWriter(f, fieldnames=HEADERS.keys())
        writer.writeheader()
        writer.writerows(rows)
    print(f"Saved {len(rows)} faculty records to {CSV_PATH}")
    print(f"Photos are in {IMAGE_DIR}")


if __name__ == "__main__":
    main()
