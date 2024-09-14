import sys
from transformers import pipeline
summarizer = pipeline("summarization",model="Falconsai/text_summarization")
result = summarizer(sys.argv[1],max_length = 40,min_length = 0,do_sample = False)[0]
print(result['summary_text'])