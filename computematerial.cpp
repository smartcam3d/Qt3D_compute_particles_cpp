//=============================================================================================================
/**
* @file     computematerial.cpp
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*
* @brief    ComputeMaterial class definition.
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "computematerial.h"


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================
#include <Qt3DRender>

//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace COMPUTESHADER;
using namespace Qt3DRender;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE GLOBAL METHODS
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

ComputeMaterial::ComputeMaterial(Qt3DCore::QNode *parent)
: QMaterial(parent)
, m_fFinalCollisonFactor(0.2f)
, m_fParticleStep(0.4f)
, m_pEffect(new QEffect)
, m_pParticleStepParameter(new QParameter(QStringLiteral("particleStep"), m_fParticleStep))
, m_pCollisionParameter(new QParameter(QStringLiteral("finalCollisionFactor"), m_fFinalCollisonFactor))
, m_pParticlesParameter(new QParameter)
, m_pComputeShader(new QShaderProgram)
, m_pComputeRenderPass(new QRenderPass)
, m_pComputeFilterKey(new QFilterKey)
, m_pComputeTechnique(new QTechnique)
, m_pDrawShader(new QShaderProgram)
, m_pDrawRenderPass(new QRenderPass)
, m_pDrawFilterKey(new QFilterKey)
, m_pDrawTechnique(new QTechnique)
{
    this->init();
}


//*************************************************************************************************************

void ComputeMaterial::setVertexBuffer(QPointer<QBuffer> inBuffer)
{
    m_pParticlesParameter->setName(QStringLiteral("Particles"));
    //
    QVariant tempVariant;
    tempVariant.setValue(particleBuffer);
    m_pParticlesParameter->setValue(tempVariant);
    m_pComputeRenderPass->addParameter(m_pParticlesParameter);
}


//*************************************************************************************************************

void ComputeMaterial::init()
{
    //Compute part
    //Set shader
    m_pComputeShader->setComputeShaderCode(QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/particles.csh"))));

    m_pComputeRenderPass->setShaderProgram(m_pComputeShader);

    //Set OpenGL version
    m_pComputeTechnique->graphicsApiFilter()->setApi(QGraphicsApiFilter::OpenGL);
    m_pComputeTechnique->graphicsApiFilter()->setMajorVersion(4);
    m_pComputeTechnique->graphicsApiFilter()->setMinorVersion(3);
    m_pComputeTechnique->graphicsApiFilter()->setProfile(QGraphicsApiFilter::CoreProfile);

    //Set filter Keys
    m_pComputeFilterKey->setName(QStringLiteral("type"));
    m_pComputeFilterKey->setValue(QStringLiteral("compute"));

    //Add to technique
    m_pComputeTechnique->addFilterKey(m_pComputeFilterKey);
    m_pComputeTechnique->addRenderPass(m_pComputeRenderPass);

    //Draw part
    //Set shader
    m_pDrawShader->setVertexShaderCode(QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/particles.vert"))));
    m_pDrawShader->setFragmentShaderCode(QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/particles.frag"))));

    m_pDrawRenderPass->setShaderProgram(m_pDrawShader);

    //Set OpenGL version
    m_pDrawTechnique->graphicsApiFilter()->setApi(QGraphicsApiFilter::OpenGL);
    m_pDrawTechnique->graphicsApiFilter()->setMajorVersion(4);
    m_pDrawTechnique->graphicsApiFilter()->setMinorVersion(3);
    m_pDrawTechnique->graphicsApiFilter()->setProfile(QGraphicsApiFilter::CoreProfile);

    //Set filter Keys
    m_pDrawFilterKey->setName(QStringLiteral("type"));
    m_pDrawFilterKey->setValue(QStringLiteral("draw"));

    //Add to technique
    m_pDrawTechnique->addFilterKey(m_pDrawFilterKey);
    m_pDrawTechnique->addRenderPass(m_pDrawRenderPass);

    //Effect
    m_pEffect->addTechnique(m_pComputeTechnique);
    m_pEffect->addTechnique(m_pDrawTechnique);

    //Add to material
    this->addParameter(m_pParticleStepParameter);
    this->addParameter(m_pCollisionParameter);

    this->setEffect(m_pEffect);
}


//*************************************************************************************************************
