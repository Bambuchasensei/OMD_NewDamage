class PPERequester_OMD_NewDamageOrgans extends PPERequester_GameplayBase
{
	protected float m_VignetteTarget = 0;
	protected float m_VignetteCurrent = 0;
	protected float m_BlurTarget = 0;
	protected float m_BlurCurrent = 0;
	protected float m_LerpSpeed = 1.5;


	void SetOrganEffects(float vignette, float blur)
	{
		m_VignetteTarget = vignette;
		m_BlurTarget = blur;
		if (m_VignetteTarget < 0)
		{
			m_VignetteTarget = 0;
		}

		if (m_BlurTarget < 0)
		{
			m_BlurTarget = 0;
		}

		if (m_VignetteTarget > 2)
		{
			m_VignetteTarget = 2;
		}

		if (m_BlurTarget > 1)
		{
			m_BlurTarget = 1;
		}
	}


	override protected void OnStop(Param par = null)
	{
		super.OnStop();
		m_VignetteTarget = 0;
		m_VignetteCurrent = 0;
		m_BlurTarget = 0;
		m_BlurCurrent = 0;
		SetTargetValueFloat(PostProcessEffectType.Glow, PPEGlow.PARAM_VIGNETTE, false, 0, PPEGlow.L_OMD_ORGAN, PPOperators.HIGHEST);
		SetTargetValueColor(PostProcessEffectType.Glow, PPEGlow.PARAM_VIGNETTECOLOR, {0.0, 0.0, 0.0, 0.0}, PPEGlow.L_OMD_ORGAN, PPOperators.LOWEST);
		SetTargetValueFloat(PostProcessEffectType.GaussFilter, PPEGaussFilter.PARAM_INTENSITY, false, 0, PPEGaussFilter.L_OMD_ORGAN, PPOperators.HIGHEST);
	}


	override void OnUpdate(float delta)
	{
		super.OnUpdate(delta);
		float step;
		step = delta * m_LerpSpeed;
		if (step < 0)
		{
			step = 0;
		}

		if (step > 1)
		{
			step = 1;
		}

		m_VignetteCurrent = m_VignetteCurrent + ((m_VignetteTarget - m_VignetteCurrent) * step);
		m_BlurCurrent = m_BlurCurrent + ((m_BlurTarget - m_BlurCurrent) * step);
		if (m_VignetteCurrent < 0.001 && m_VignetteTarget <= 0)
		{
			m_VignetteCurrent = 0;
		}

		if (m_BlurCurrent < 0.001 && m_BlurTarget <= 0)
		{
			m_BlurCurrent = 0;
		}

		SetTargetValueFloat(PostProcessEffectType.Glow, PPEGlow.PARAM_VIGNETTE, false, m_VignetteCurrent, PPEGlow.L_OMD_ORGAN, PPOperators.HIGHEST);
		SetTargetValueColor(PostProcessEffectType.Glow, PPEGlow.PARAM_VIGNETTECOLOR, {0.0, 0.0, 0.0, 0.0}, PPEGlow.L_OMD_ORGAN, PPOperators.LOWEST);
		SetTargetValueFloat(PostProcessEffectType.GaussFilter, PPEGaussFilter.PARAM_INTENSITY, false, m_BlurCurrent, PPEGaussFilter.L_OMD_ORGAN, PPOperators.HIGHEST);
	}

};
