# utils/logger.py
import logging
import os
import sys

class FlushStreamHandler(logging.StreamHandler):
    """StreamHandler that always flushes immediately."""
    def emit(self, record):
        super().emit(record)
        self.flush()

def get_logger(log_dir="results", log_file="train.log"):
    os.makedirs(log_dir, exist_ok=True)
    log_path = os.path.join(log_dir, log_file)

    logger = logging.getLogger("VolleyballLogger")
    logger.setLevel(logging.INFO)

    # Avoid duplicate handlers
    if not logger.handlers:
        # File handler (writes to log file)
        fh = logging.FileHandler(log_path, mode="w")
        # Custom stream handler (flushes to terminal immediately)
        ch = FlushStreamHandler(sys.stdout)

        formatter = logging.Formatter("%(asctime)s - %(levelname)s - %(message)s")
        fh.setFormatter(formatter)
        ch.setFormatter(formatter)

        logger.addHandler(fh)
        logger.addHandler(ch)

    return logger
