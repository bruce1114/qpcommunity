# ./main_only_number <
import commands

#exp1
# for i in range(3,8):
#     with open("tempinput.txt","w") as fout:
#         # fout.write("enron_day "+str(i)+" 4 5 3\n")
#         # fout.write("enron_day "+str(i)+" 4 15 3\n")
#         # fout.write("enron_day 4 "+str(i)+" 5 3\n")
#         fout.write("enron_day 4 "+str(i)+" 15 3\n")
#     run="./main_only_number < tempinput.txt >> nohup.out"
#     print(commands.getstatusoutput(run))

#exp4
# prog=['./main_plus_10_24',]
# # dataset=['dblp','imdb']
# dataset=['enron_day',]
# for dset in dataset:
#     for i in [3,]:
#         for k in range(3,7):
#             with open("tempinput1.txt","w") as fout:
#             # fout.write("enron_day "+str(i)+" 4 5 3\n")
#             # fout.write("enron_day "+str(i)+" 4 15 3\n")
#             # fout.write("enron_day 4 "+str(i)+" 5 3\n")
#                 fout.write(dset+" "+str(k)+" 6 15 "+str(i)+"\n")
            
#             for p in prog:
#                 run=p+" < tempinput1.txt >> nohup1.out"
#                 print(commands.getstatusoutput(run))

# exp3
# dataset=['trueseq_100','trueseq_200']
# for dset in dataset:
#     for sigma in range(4,11,2):
#         for e in range(5,21,5):
#             with open("tempinput.txt","w") as fout:
#             # fout.write("enron_day "+str(i)+" 4 5 3\n")
#             # fout.write("enron_day "+str(i)+" 4 15 3\n")
#             # fout.write("enron_day 4 "+str(i)+" 5 3\n")
#                 fout.write(dset+"\n")
#                 fout.write(str(sigma)+" "+str(e)+'\n')
            
#             if dset=='trueseq_200' and sigma>=8 and e==20:
#                 with open("nohup.out","a") as fout:
#                     fout.write("INF\n")
#                 continue
#             run='./test_weds_10_21 < tempinput.txt >> nohup.out'
#             print(commands.getstatusoutput(run))


# exp6
# dataset=['dblp','enron_day']
# prog=['./LMPC_direct_10_22']
# for dset in dataset:
#     for p in prog:
#         for e in range(5,21,5):
#             with open("tempinput.txt","w") as fout:
#                 fout.write(dset+" 4 4 "+str(e))
#                 if p=='./main_plus_10_21':
#                     fout.write(" 3")
#                 fout.write('\n')
#             # fout.write("enron_day "+str(i)+" 4 15 3\n")
#             # fout.write("enron_day 4 "+str(i)+" 5 3\n")
#                 # fout.write(dset+"\n")
#                 # fout.write(str(sigma)+" "+str(e)+'\n')
#             run=p+" < tempinput.txt >> nohup.out"
#             print(commands.getstatusoutput(run))


# run='./test'
# print(commands.getstatusoutput(run))


#exp4 extend
# prog=['./main_plus_10_24','./main_set_10_24']
# dataset=['dblp','enron_day','imdb']
# for dset in dataset:
#     for i in [3,]:
#         for p in prog:
#             for e in range(5,21,5):
#                 if dset=='imdb' and e>=10:
#                     with open("nohup.out","a") as fout:
#                         fout.write("noneed for imdb\n")
#                     continue
#                 with open("tempinput.txt","w") as fout:
#                 # fout.write("enron_day "+str(i)+" 4 5 3\n")
#                 # fout.write("enron_day "+str(i)+" 4 15 3\n")
#                 # fout.write("enron_day 4 "+str(i)+" 5 3\n")
#                     fout.write(dset+" 4 4 "+str(e)+" "+str(i)+"\n")
#                 run=p+"< tempinput.txt >> nohup.out"
#                 print(commands.getstatusoutput(run))

# exp vary k and sigma
dataset=['imdb',]
prog=['./main_set_10_24','./main_plus_10_24']
for dset in dataset:
    for i in [1,3]:
        for p in prog:
            for k in [4,5,6]:
                for sig in range(4,11,2):
                    for e in [5,]:
                        with open("tempinput1.txt","w") as fout:
                            fout.write(dset+" "+str(k)+" "+str(sig)+" "+str(e)+" "+str(i)+"\n")
                        
                        run=p+" < tempinput1.txt >> nohup1.out"

                        with open("nohup1.out","a") as fout:
                            fout.write("para: "+dset+" "+str(k)+" "+str(sig)+" "+str(e)+" "+str(i)+"\n")
                        print(commands.getstatusoutput(run))