#=============================================================================
#
#  Copyright (c) Kitware, Inc.
#  All rights reserved.
#  See LICENSE.txt for details.
#
#  This software is distributed WITHOUT ANY WARRANTY; without even
#  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#  PURPOSE.  See the above copyright notice for more information.
#
#=============================================================================
import os, sys
import unittest
import smtk
import smtk.testing
from smtk.simple import *

class UnitMeshOperator(unittest.TestCase):

  def setUp(self):
    self.mgr = smtk.model.Manager.create()
    self.sess = self.mgr.createSession('discrete')
    SetActiveSession(self.sess)
    self.modelFile = os.path.join(smtk.testing.DATA_DIR, 'cmb', 'test2D.cmb')
    self.models = Read(self.modelFile)

  def testMeshing2D(self):
    #json = smtk.io.ExportJSON.fromModelManager(mgr, smtk.io.JSON_DEFAULT)
    #jfile = open('/tmp/meshedTess.json', 'w')
    #print >>jfile, json
    #jfile.close()

    mesher = self.sess.op('mesh')
    # Set mesher attributes here...
    print 'About to operate...'
    result = mesher.operate()
    #self.assertEqual(
    #    result.findInt('outcome').value(0),
    #    smtk.model.OPERATION_SUCCEEDED)

    #json = smtk.io.ExportJSON.fromModelManager(mgr, smtk.io.JSON_DEFAULT)
    #jfile = open('/tmp/meshedTess.json', 'w')
    #print >>jfile, json
    #jfile.close()

if __name__ == '__main__':
  smtk.testing.process_arguments()
  unittest.main()
