#execute(r"find . -name "*.cc" -print | wc")

from subprocess import *
# p = subprocess.Popen(['find', '.', '-name', "*.cc", "-print", "|", "xargs", "wc"], 
#     stdout=subprocess.PIPE, 
#     stderr=subprocess.PIPE,
#     shell=True)
# out, err = p.communicate()
# print out


p1 = Popen(['find', '.', '-name', "*.cc", "-print"], stdout=PIPE)
p2 = Popen(["xargs", "wc"], stdin=p1.stdout, stdout=PIPE)
p1.stdout.close()  # Allow p1 to receive a SIGPIPE if p2 exits.
output = p2.communicate()[0]
print output