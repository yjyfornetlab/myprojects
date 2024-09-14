import sys
from transformers import AutoTokenizer, MarianMTModel

model_name = "Helsinki-NLP/opus-mt-en-zh"
model = MarianMTModel.from_pretrained(model_name)
tokenizer = AutoTokenizer.from_pretrained(model_name)
batch = tokenizer([sys.argv[1]],return_tensors = "pt")

generated_ids = model.generate(**batch)
result = tokenizer.batch_decode(generated_ids,skip_special_tokens=True)[0]
print(result)