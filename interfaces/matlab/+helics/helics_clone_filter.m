function v = helics_clone_filter()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 1936535388);
  end
  v = vInitialized;
end
