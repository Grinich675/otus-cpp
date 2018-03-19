#!/usr/bin/env python3
import sys
import string
import random

def static_block(block_size,command_len):
	transaction = []
	for _ in range(block_size):
		command = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(command_len))
		transaction.append(command+'\n')
	return transaction

def dynamic_block(command_len):
	block_size = random.randint(3, 10)
	transaction = ['{\n']
	for _ in range(block_size):
		command = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(command_len))
		transaction.append(command+'\n')
	transaction.append('}\n')
	return transaction

transaction_count = 100000
command_len = 6
s_trans_len = 5
modes = ['static','dynamic']

fo = open("commands", "w+")

if len(sys.argv) > 1 :
 transaction_count = int(sys.argv[1])

for _ in range(transaction_count):
	#mode = random.choice(modes)
	if random.choice(modes) == 'static':
		fo.writelines( static_block(s_trans_len,command_len) )
		#print(static_block(s_trans_len,command_len) )
	else:
		#print(dynamic_block(command_len) )
		fo.writelines(dynamic_block(command_len))
fo.close()


