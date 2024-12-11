void bank(char *model, char *instance)
{
  begBuildCompactModel();

  if (!gns_ModelVisited(model))
  {
    gns_MarkModelVisited(model);
    begSaveModel();
  }
}
