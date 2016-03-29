import os
import sys

searchKeys = ['Notification', 'UIKIT_EXTERN']
# searchKeys = [r'NS_AVAILABLE_IOS(9_0)']

searchPath = '/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk/System/Library/Frameworks'

def matchAllKeys(arrs, line):
     for key in arrs:
          if key not in line:
               return False
     return True

def findText(filePath):
     res = []
     f = open(filePath)
     for line in f:
          if matchAllKeys(searchKeys, line):
               res.append( os.path.split(filePath)[1] + '-------------'  + line)

     return res

def traversal(path):

     ls = []
     for parent, _, files in os.walk(path):

          for fileName in files:
               if '.h' in fileName:
                    lines = findText(os.path.join(parent, fileName))
                    ls += lines
     return ls

if __name__ == '__main__':

     ls = traversal(searchPath)

     fileName = 'result.txt'
     f = open(fileName, 'w')
     for line in ls:
          f.write(line)
     f.close()

