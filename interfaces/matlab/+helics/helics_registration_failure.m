function v = helics_registration_failure()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 1936535355);
  end
  v = vInitialized;
end
