#!/usr/bin/env python


import os
import subprocess
import tempfile
import unittest

class TestFptf(unittest.TestCase):
    def setUp(self) -> None:
        # Create a temporary file in the current directory.
        with tempfile.NamedTemporaryFile(dir=os.getcwd(), delete=False) as self.temp_file:
            pass
        
    
    def tearDown(self) -> None:
        # Delete the temporary file if it still exists.
        if os.path.exists(self.temp_file.name):
            os.remove(self.temp_file.name)
    
    def run_test(self, quick_exit, test_app):
        quick_exit_arg = 'true' if quick_exit else 'false'

        # Sanity check that the temporary file exists before the test_app is spawned.
        self.assertTrue(os.path.exists(self.temp_file.name), msg="Temporary file not created before test_app was spawned.")

        # Spawn the test_app process.
        test_app_path = os.path.join(os.path.dirname(__file__), test_app)
        proc = subprocess.Popen([test_app_path, self.temp_file.name, quick_exit_arg], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = proc.communicate()
        self.assertEqual(proc.returncode, 0, msg=f'Process exited with code {proc.returncode}.\nstdout: {stdout}\nstderr: {stderr}')

        # Check that the temporary file is deleted after the spawned process exits.
        self.assertFalse(os.path.exists(self.temp_file.name), msg=f'Temporary file not deleted after test_app exited.\nstdout: {stdout}\nstderr: {stderr}')

    def test_cpp_quick_exit(self):
        self.run_test(True, 'test_app_cpp')

    def test_cpp_normal_exit(self):
        self.run_test(False, 'test_app_cpp')

    def test_c_quick_exit(self):
        self.run_test(True, 'test_app_c')

    def test_c_normal_exit(self):
        self.run_test(False, 'test_app_c')