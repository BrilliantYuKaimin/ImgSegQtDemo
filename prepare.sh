if [ -d model ]
then
  echo "在 model 目录已经存在的情况下，此脚本将直接结束。您可以在删除或重命名它之后再运行此脚本。"
else
  wget https://paddleseg.bj.bcebos.com/dygraph/demo/stdc1seg_infer_model.tar.gz
  tar -xf stdc1seg_infer_model.tar.gz
  mv stdc1seg_infer_model model
  rm stdc1seg_infer_model.tar.gz
  rm model/deploy.yaml
  rm model/model.pdiparams.info
fi
