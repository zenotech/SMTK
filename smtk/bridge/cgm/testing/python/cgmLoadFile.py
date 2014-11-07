import sys
import os
sys.path.append('/Users/dcthomp/Build/SMTK/public')
sys.path.append('/Users/dcthomp/Build/SMTK/public/lib')
print sys.path

import smtk
try:
  import cgmsmtk
except:
  print "Expected"

ddir = sys.argv[-1]

mgr = smtk.model.Manager.create()
brg = smtk.model.Manager.createBridge('cgm')
mgr.registerBridgeSession(brg)

readop = brg.op('read', mgr)
readop.findFile('filename').setValue(os.path.join(ddir,'cgm','62_shaver1.brep'))
# Note that we could set the file type here:
#    readop.findString('filetype').setValue('OCC')
# but there is no need to in this case as it will be
# correctly inferred from the filename.
res = readop.operate()