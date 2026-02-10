#include "pch.h"
#include "Componant.h"

Matrix4x4f& Transform::GetWorld()
{
	return m_world;
	// TODO: insérer une instruction return ici
}

Matrix4x4f& Transform::GetInvWorld()
{
	return m_invworld;
	// TODO: insérer une instruction return ici
}
