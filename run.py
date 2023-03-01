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
# prog=['./main_set_10_24','./main_plus_10_24']
# dataset=['enron_day',]
# for dset in dataset:
#     strategy=[1,3]
#     if dset=='enron_day':
#         strategy=[3,1]
#     for i in strategy:
#         with open("tempinput.txt","w") as fout:
#         # fout.write("enron_day "+str(i)+" 4 5 3\n")
#         # fout.write("enron_day "+str(i)+" 4 15 3\n")
#         # fout.write("enron_day 4 "+str(i)+" 5 3\n")
#             fout.write(dset+" 3 6 5 "+str(i)+"\n")
        
#         for p in prog:
#             run=p+" < tempinput.txt >> nohup.out"
#             print(commands.getstatusoutput(run))

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

# exp vary k and sigma
# dataset=['dblp','enron_day']
# prog=['./main_plus_10_24','./main_set_10_24']
# for dset in dataset:
#     for i in [1,3]:
#         for p in prog:
#             for k in [4,5,6]:
#                 for sig in [4,6,8,10]:
#                     with open("tempinput1.txt","w") as fout:
#                         fout.write(dset+" "+str(k)+" "+str(sig)+" "+str(5)+" "+str(i)+"\n")
                    
#                     run=p+" < tempinput1.txt >> nohup1.out"
#                     print(commands.getstatusoutput(run))

#exp4 extend
# prog=['./main_set_10_24','./main_plus_10_24']
# dataset=['dblp','imdb','enron_day']
# for dset in dataset:
#     strategy=[1,3]
#     if dset=='enron_day':
#         strategy=[3,1]
#     for i in strategy:
#         for p in prog:
#             for e in range(10,21,5):
#                 with open("tempinput.txt","w") as fout:
#                 # fout.write("enron_day "+str(i)+" 4 5 3\n")
#                 # fout.write("enron_day "+str(i)+" 4 15 3\n")
#                 # fout.write("enron_day 4 "+str(i)+" 5 3\n")
#                     fout.write(dset+" 4 6 "+str(e)+" "+str(i)+"\n")
#                 run=p+" < tempinput.txt >> nohup.out"
#                 print(commands.getstatusoutput(run))


# core and clique all datasets
dataset=['enron_day']
prog=['./main_core_clique_time_plus_moretime',]
# prog=['./main_core_clique_time_minus_moretime','./main_core_clique_time_plus_moretime']
for dset in dataset:
    strategy=[3,]
    # if dset=='lkml':
    #     strategy=[5,4]
    for i in strategy:
        kval=4
        if i>=4:
            kval=kval-1
        for p in prog:
            for k in [kval,]:
                for sig in range(3,8):
                    for e in range(10,11,10):
                        with open("tempinput.txt","w") as fout:
                            fout.write(dset+" "+str(k)+" "+str(sig)+" "+str(e)+" "+str(i)+"\n")
                        
                        run=p+" < tempinput.txt >> nohup2.out"
                        with open("nohup2.out","a") as fout:
                            fout.write("para: "+dset+" "+str(k)+" "+str(sig)+" "+str(e)+" "+str(i)+"\n")
                        print(commands.getstatusoutput(run))