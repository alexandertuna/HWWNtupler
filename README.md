# HWWNtupler

To get started, try:

```bash
rel="2.3.45"
rcSetup Base,${rel}

git clone git@github.com:UCATLAS/xAODAnaHelpers.git
python xAODAnaHelpers/scripts/checkoutASGtags.py ${rel}

git clone git@github.com:alexandertuna/HWWNtupler.git

rc find_packages
rc compile

test_file="/n/atlasfs/atlasdata/tuna/hww/p2495/mc15_13TeV.341080.PowhegPythia8EvtGen_CT10_AZNLOCTEQ6L1_VBFH125_WWlvlv_EF_15_5.merge.DAOD_HIGG3D1.e3871_s2608_s2183_r7326_r6282_p2495_tid07374656_00/DAOD_HIGG3D1.07374656._000001.pool.root.1"

xAH_run.py --files ${test_file} --nevents 10000 --config HWWNtupler/scripts/config.py --inputTag '*DAOD*' -f --submitDir 'test' direct
```

