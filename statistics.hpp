struct Statistics
{
  double mean_;
  double sigma_;
  double mean_err_;
  Statistics(double mean = 0., double sigma = 0., double mean_err = 0.)
      : mean_{mean}
      , sigma_{sigma}
      , mean_err_{mean_err}
  {}
};
