/*
 * Copyright (C) 2021 LibXZR <xzr467706992@163.com>.
 */

#include <linux/cpufreq.h>

#define FREQ_LIMIT_LP 0
#define FREQ_LIMIT_PERF 1766400
#define FREQ_LIMIT_PRIME 2073600

static bool do_freq_limit = true;

static inline void limit_max(struct cpufreq_policy *policy,
			unsigned int freq)
{
	if (!freq)
		return;

	if (policy->max > freq)
		policy->max = freq;
}

void restrict_frequency(struct cpufreq_policy *policy)
{
	if (!do_freq_limit)
		return;

	if (cpumask_test_cpu(policy->cpu, cpu_lp_mask))
		limit_max(policy, FREQ_LIMIT_LP);
	else if (cpumask_test_cpu(policy->cpu, cpu_perf_mask))
		limit_max(policy, FREQ_LIMIT_PERF);
	else
		limit_max(policy, FREQ_LIMIT_PRIME);
}

void set_frequency_limit(bool enable) {
	int ret;

	do_freq_limit = enable;

	ret = cpufreq_policy_reset_limit();
	if (ret)
		pr_err("Unable to reset policy limits\n");
}
